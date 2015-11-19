bl_info = {
    "name": "Treeface addon panel",
    "category": "Object",
}

import bpy
import bgl
import mathutils
import math
import itertools

class TreefaceScenePanel(bpy.types.Panel):
    bl_label       = "Treeface Properties"
    bl_space_type  = "PROPERTIES"
    bl_context     = "scene"
    bl_region_type = "WINDOW"

    def draw(self, context):
        layout = self.layout
        scene = context.scene

        layout.prop_search(scene, "treeface_global_light", context.scene, "lamps")
        layout.prop(scene, "treeface_toplevel_width")
        layout.prop(scene, "treeface_toplevel_height")

class TreefaceObjectPanel(bpy.types.Panel):
    bl_label       = "Treeface Properties"
    bl_space_type  = "PROPERTIES"
    bl_context     = "object"
    bl_region_type = "WINDOW"

    def draw_header(self, context):
        layout = self.layout
        obj = context.object
    
    def draw(self, context):
        obj = context.object

        layout = self.layout
        
        if obj.type == 'MESH':
            layout.prop(obj, 'is_treeface_visual_object')
        elif obj.type == 'EMPTY':
            layout.prop(obj, 'is_treeface_scene_node')
        
            layout_widget = layout.column()
            layout_widget.prop(obj, "is_treeface_widget")
    
            layout_widget_prop = layout_widget.column()
            layout_widget_prop.prop(obj, "treeface_widget_type")
            
            layout_widget_size = layout_widget_prop.row()
            layout_widget_size.prop(obj, "treeface_widget_width")
            layout_widget_size.prop(obj, "treeface_widget_height")
            
            layout_widget_prop.separator()
            
            #
            if obj.treeface_widget_type == 'TREEFACE_WIDGET_TRIGGER' or obj.treeface_widget_type == 'TREEFACE_WIDGET_VALUE':
                layout_widget_prop.prop(obj, 'treeface_widget_motion_type')
                
                if obj.treeface_widget_motion_type == 'TREEFACE_WIDGET_MOTION_TRANSLATE':
                    layout_widget_prop.prop(obj, 'treeface_widget_motion_limit_trans')
                elif obj.treeface_widget_motion_type == 'TREEFACE_WIDGET_MOTION_ROTATE':
                    layout_widget_prop.prop(obj, 'treeface_widget_motion_limit_rot')
            
                if obj.treeface_widget_type == 'TREEFACE_WIDGET_VALUE':
                    layout_widget_prop.prop(obj, 'treeface_widget_motion_child_grids')
                    layout_widget_prop.prop(obj, 'treeface_widget_num_motion_child')
                
                layout_widget_prop.prop_search(obj, 'treeface_widget_motion_child', bpy.data, 'objects')
                
                if obj.treeface_widget_num_motion_child == 2:
                    layout_widget_prop.prop_search(obj, 'treeface_widget_motion_child_2', bpy.data, 'objects')
            
            #
            if obj.is_treeface_scene_node:
                layout_widget.enabled = True
            else:
                layout_widget.enabled = False

            if obj.is_treeface_widget:
                layout_widget_prop.enabled = True
            else:
                layout_widget_prop.enabled = False

unit_z = mathutils.Vector((0, 0, 1))
unit_x = mathutils.Vector((1, 0, 0))
def paint_motion_child_things(obj, child_pos):
    if obj.treeface_widget_motion_type == 'TREEFACE_WIDGET_MOTION_TRANSLATE':
        offset       = mathutils.Vector(obj.treeface_widget_motion_limit_trans)
        direct       = offset.normalized()
        ortho_direct = unit_x
        
        if direct != unit_z:
            ortho_direct = direct.cross(unit_z)
            
        end_pos      = child_pos + offset
        
        arrow_root_len = offset.length - 1
        arrow_pos1 = child_pos + direct * arrow_root_len + ortho_direct * 0.5
        arrow_pos2 = child_pos + direct * arrow_root_len + ortho_direct * -0.5
        
        bgl.glBegin(bgl.GL_LINES)
        bgl.glVertex3f(child_pos.x,  child_pos.y,  child_pos.z)
        bgl.glVertex3f(end_pos.x,    end_pos.y,    end_pos.z)
        
        bgl.glVertex3f(arrow_pos1.x, arrow_pos1.y, arrow_pos1.z)
        bgl.glVertex3f(end_pos.x,    end_pos.y,    end_pos.z)
        
        bgl.glVertex3f(arrow_pos2.x, arrow_pos2.y, arrow_pos2.z)
        bgl.glVertex3f(end_pos.x,    end_pos.y,    end_pos.z)
        
        if obj.treeface_widget_type == 'TREEFACE_WIDGET_VALUE' and obj.treeface_widget_motion_child_grids > 0:
            grid_len = offset.length / obj.treeface_widget_motion_child_grids
            
            i = 1
            while i < obj.treeface_widget_motion_child_grids:
                pos1 = child_pos + direct * i * grid_len
                pos2 = pos1 + ortho_direct * 0.5
                bgl.glVertex3f(pos1.x, pos1.y, pos1.z)
                bgl.glVertex3f(pos2.x, pos2.y, pos2.z)
                i += 1 
        
        bgl.glEnd()
        
    elif obj.treeface_widget_motion_type == 'TREEFACE_WIDGET_MOTION_ROTATE':
        direct = mathutils.Vector((obj.treeface_widget_motion_limit_rot[1], obj.treeface_widget_motion_limit_rot[2], obj.treeface_widget_motion_limit_rot[3]))
        direct.normalize()
        
        # get two ortholog directions
        ortho1 = direct.cross(unit_z)
        if ortho1.length == 0:
            ortho1 = unit_x
        ortho2 = direct.cross(ortho1)
        ortho1.normalize()
        ortho2.normalize()
        
        print("direct length:",str(direct.length))
        print("o1 length:",str(ortho1.length))
        print("o2 length:",str(ortho2.length))
        
        # plot axis
        axis_end     = child_pos + direct * 3
        bgl.glBegin(bgl.GL_LINES)
        bgl.glVertex3f(child_pos.x,  child_pos.y,  child_pos.z)
        bgl.glVertex3f(axis_end.x,  axis_end.y,  axis_end.z)
        bgl.glEnd()
        
        # plot begin radius
        r_begin = child_pos + ortho1 * 3
        bgl.glBegin(bgl.GL_LINE_STRIP)
        bgl.glVertex3f(child_pos.x,  child_pos.y,  child_pos.z)
        bgl.glVertex3f(r_begin.x, r_begin.y, r_begin.z)
        
        # plot arc
        arc_step = 3.1415926 / 36
        curr_angle = 0
        while curr_angle < obj.treeface_widget_motion_limit_rot[0]:
            pos = child_pos + ortho1 * math.cos(curr_angle) * 3 + ortho2 * math.sin(curr_angle) * 3
            bgl.glVertex3f(pos.x, pos.y, pos.z)
            curr_angle += arc_step
        bgl.glEnd()
        
        # plot grids
        if obj.treeface_widget_type == 'TREEFACE_WIDGET_VALUE' and obj.treeface_widget_motion_child_grids > 0:
            grid_step = obj.treeface_widget_motion_limit_rot[0] / obj.treeface_widget_motion_child_grids
            
            bgl.glBegin(bgl.GL_LINES)
            i = 1
            while i < obj.treeface_widget_motion_child_grids:
                angle = grid_step * i
                radius_direct = ortho1 * math.cos(angle) + ortho2 * math.sin(angle)
                p1 = child_pos + radius_direct * 3.2
                p2 = child_pos + radius_direct * 2.8
                 
                bgl.glVertex3f(p1.x, p1.y, p1.z)
                bgl.glVertex3f(p2.x, p2.y, p2.z)
                 
                i += 1
            
            bgl.glEnd()
        
    else:
        self.report({'ERROR_INVALID_INPUT'}, "invalid motion type: " + obj.treeface_widget_motion_type)

def repaint_treeface_things():
    prev_color = bgl.Buffer(bgl.GL_FLOAT, [4])
    bgl.glGetFloatv(bgl.GL_CURRENT_COLOR, prev_color)
    
    color1 = [0.3, 0.7, 0.4]
    color2 = [0, 0.15, 0.07]
    
    for scene in bpy.data.scenes:
        w = scene.treeface_toplevel_width
        h = scene.treeface_toplevel_height
        
        bgl.glColor3f(color2[0], color2[1], color2[2])
        
        bgl.glBegin(bgl.GL_LINE_STRIP)
        bgl.glVertex3f(0, 0, 0)
        bgl.glVertex3f(w, 0, 0)
        bgl.glVertex3f(w, h, 0)
        bgl.glVertex3f(0, h, 0)
        bgl.glVertex3f(0, 0, 0)
        bgl.glEnd()
    
    for obj in bpy.data.objects:
        if not (obj.is_treeface_scene_node and obj.is_treeface_widget):
            continue
        
        if obj.select:
            bgl.glColor3f(color1[0], color1[1], color1[2])
        else:
            bgl.glColor3f(color2[0], color2[1], color2[2])
        
        # plot widget bounding box
        w = obj.treeface_widget_width
        h = obj.treeface_widget_height
        
        obj_pos = obj.matrix_world.to_translation();

        bgl.glBegin(bgl.GL_LINE_STRIP)
        bgl.glVertex3f(obj_pos.x,   obj_pos.y,   obj_pos.z)
        bgl.glVertex3f(obj_pos.x+w, obj_pos.y,   obj_pos.z)
        bgl.glVertex3f(obj_pos.x+w, obj_pos.y+h, obj_pos.z)
        bgl.glVertex3f(obj_pos.x,   obj_pos.y+h, obj_pos.z)
        bgl.glVertex3f(obj_pos.x,   obj_pos.y,   obj_pos.z)
        bgl.glEnd()

        # plot motion child
        if obj.treeface_widget_type == 'TREEFACE_WIDGET_TRIGGER' or obj.treeface_widget_type == 'TREEFACE_WIDGET_VALUE':
            motion_child_name =  obj.treeface_widget_motion_child
            
            if len(motion_child_name) and motion_child_name in bpy.data.objects:
                motion_child = bpy.data.objects[motion_child_name]
                child_pos    = motion_child.matrix_world.to_translation()
                paint_motion_child_things(obj, child_pos)
                
                
    # reset color
    bgl.glColor4f(prev_color[0], prev_color[1], prev_color[2], prev_color[3])

update_handler = None
def register():
    # treeface object properties
    bpy.types.Object.is_treeface_scene_node = bpy.props.BoolProperty(
        name = "is treeface scene node",
        description = "object is used as treeface SceneNode"
    )
    bpy.types.Object.is_treeface_visual_object = bpy.props.BoolProperty(
        name = "is treeface visual object",
        description = "object is used as treeface VisualObject"
    )

    bpy.types.Object.is_treeface_widget = bpy.props.BoolProperty(name        = "is treeface widget",
                                                                 description = "object is root node of a treeface widget")

    bpy.types.Object.treeface_widget_type = bpy.props.EnumProperty(name    = "widget type",
                                                                   items   = [("TREEFACE_WIDGET_BASE",    "base",           "base widget type"),
                                                                              ("TREEFACE_WIDGET_TRIGGER", "trigger",        "widget that triggers an event when clicked"),
                                                                              ('TREEFACE_WIDGET_VALUE',   'value selector', "widget that has discrete states or continuous value")],
                                                                   default = 'TREEFACE_WIDGET_BASE')

    bpy.types.Object.treeface_widget_width = bpy.props.IntProperty(name        = 'width',
                                                                   description = 'widget width',
                                                                   subtype     = 'UNSIGNED',
                                                                   default     = 20)
    
    bpy.types.Object.treeface_widget_height = bpy.props.IntProperty(name        = 'height',
                                                                    description = 'widget height',
                                                                    subtype     = 'UNSIGNED',
                                                                    default     = 20)
    
    # motion children
    bpy.types.Object.treeface_widget_motion_child_grids = bpy.props.IntProperty(name = 'number of motion grids',
                                                                                min = 1,
                                                                                default = 0)
    
    bpy.types.Object.treeface_widget_motion_type = bpy.props.EnumProperty(name        = "motion type",
                                                                          description = "how does the motion node moves",
                                                                          items       = [('TREEFACE_WIDGET_MOTION_TRANSLATE', 'translate', ''),
                                                                                         ('TREEFACE_WIDGET_MOTION_ROTATE',    'rotate',    '')],
                                                                          default     = 'TREEFACE_WIDGET_MOTION_TRANSLATE')
    
    bpy.types.Object.treeface_widget_motion_limit_trans = bpy.props.FloatVectorProperty(name    = "translation upper bound",
                                                                                        subtype = 'TRANSLATION',
                                                                                        size    = 3,
                                                                                        step    = 0.1)
    
    bpy.types.Object.treeface_widget_motion_limit_rot = bpy.props.FloatVectorProperty(name    = "rotation upper bound",
                                                                                      subtype = 'AXISANGLE',
                                                                                      size    = 4,
                                                                                      step    = 0.1,
                                                                                      default = (3.1415926, 0, 0, 1))
    
    bpy.types.Object.treeface_widget_num_motion_child = bpy.props.IntProperty(name    = 'number of motion child',
                                                                              min     = 1,
                                                                              max     = 2,
                                                                              default = 1)
    
    bpy.types.Object.treeface_widget_motion_child = bpy.props.StringProperty(name = "motion child")

    bpy.types.Object.treeface_widget_motion_child_2 = bpy.props.StringProperty(name = "motion child 2")
    
    

    # treeface scene properties
    bpy.types.Scene.treeface_toplevel_width = bpy.props.IntProperty(
        name        = "toplevel width",
        description = "width of the toplevel widget",
        min         = 1,
        step        = 10,
        default     = 1000,
        subtype     = 'PIXEL'
    );
    bpy.types.Scene.treeface_toplevel_height = bpy.props.IntProperty(
        name        = "toplevel height",
        description = "height of the toplevel widget",
        min         = 1,
        step        = 10,
        default     = 600,
        subtype     = 'PIXEL'
    );
    bpy.types.Scene.treeface_global_light = bpy.props.StringProperty(name="global light", description="selected object will be used as global light")

    # register GUI panels
    bpy.utils.register_class(TreefaceObjectPanel)
    bpy.utils.register_class(TreefaceScenePanel)
    
    global update_handler
    update_handler = bpy.types.SpaceView3D.draw_handler_add(repaint_treeface_things, (), 'WINDOW', 'POST_VIEW')

def unregister():
    bpy.utils.unregister_class(TreefaceObjectPanel)
    bpy.utils.unregister_class(TreefaceScenePanel)
    
    global update_handler
    bpy.types.SpaceView3D.draw_handler_remove(update_handler, 'WINDOW')

if __name__ == "__main__":
    register()
