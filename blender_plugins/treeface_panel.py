bl_info = {
    "name": "Treeface addon panel",
    "category": "Object",
}

import bpy

class TreefaceScenePanel(bpy.types.Panel):
    bl_label       = "Treeface Properties"
    bl_space_type  = "PROPERTIES"
    bl_context     = "scene"
    bl_region_type = "WINDOW"

    def draw(self, context):
        layout = self.layout
        scene = context.scene

        layout.prop_search(scene, "treeface_global_light", bpy.data, "lamps")
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
        
        layout_visual = layout.box()
        layout_visual.prop(obj, "is_treeface_visual_object")
        
        layout_scene_node = layout.column()
        layout_scene_node.prop(obj, "is_treeface_scene_node")

        layout_widget = layout_scene_node.box()
        layout_widget.prop(obj, "is_treeface_widget")

        layout_widget_prop = layout_widget.column()
        layout_widget_prop.prop(obj, "treeface_widget_width")
        layout_widget_prop.prop(obj, "treeface_widget_height")
        layout_widget_prop.prop(obj, "treeface_widget_type")
        layout_widget_prop.prop(obj, "treeface_widget_custom")

        if obj.type == "MESH" or obj.type == "EMPTY":
            layout.enabled = True
        else:
            layout.enabled = False

        if obj.type == 'MESH':
            layout_visual.enabled = True
            layout_scene_node.enabled = False
        elif obj.type == 'EMPTY':
            layout_visual.enabled = False
            layout_scene_node.enabled = True
        
        if obj.is_treeface_scene_node:
            layout_widget.enabled = True
        else:
            layout_widget.enabled = False
        
        if obj.is_treeface_widget:
            layout_widget_prop.enabled = True
        else:
            layout_widget_prop.enabled = False


def get_or_generate_widget_bbox_object(object):
    # create bbox object name
    if not object.treeface_widget_bbox:
        treeface_widget_bbox_base = '_TREEFACE_WIDGET_BBOX_'+object.name
        object.treeface_widget_bbox = treeface_widget_bbox_base
        
        suffix = 1
        while bpy.data.objects.find(object.treeface_widget_bbox) != -1:
            object.treeface_widget_bbox = treeface_widget_bbox_base + '_' + str(suffix)
            suffix += 1
    
    # create bbox object if don't have
    if bpy.data.objects.find(object.treeface_widget_bbox) == -1:
        bbox_mesh = bpy.data.meshes.new(name = object.treeface_widget_bbox)
        bpy.data.objects.new(
            name        = object.treeface_widget_bbox,
            object_data = bbox_mesh,
        )
        bbox_mesh.from_pydata([(0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0)],
                              [(0, 1), (1, 2), (2, 3), (3, 0)],
                              [])
        
    
    bbox_obj = bpy.data.objects[object.treeface_widget_bbox]
    bbox_obj.parent = object
    bbox_obj.hide_render = True
    bbox_obj.hide_select = True
    bbox_obj.location = (0, 0, 0)
    
    for scene in object.users_scene:
        if scene.objects.find(bbox_obj.name) == -1:
            scene.objects.link(bbox_obj)
    
    return bbox_obj

def repaint_widget_bbox(self):
    w = self.treeface_widget_width
    h = self.treeface_widget_height
    
    bbox_obj = get_or_generate_widget_bbox_object(self)
    bbox_mesh = bbox_obj.data
    
    bbox_mesh.vertices[0].co = (w/2, h/2, 0)
    bbox_mesh.vertices[1].co = (w/2, -h/2, 0)
    bbox_mesh.vertices[2].co = (-w/2, -h/2, 0)
    bbox_mesh.vertices[3].co = (-w/2, h/2, 0)
    
    bbox_mesh.update(calc_edges = True)
    
    self.empty_draw_size = max(w, h)/2

def on_is_treeface_widget_update(self, context):
    if self.is_treeface_widget:
        repaint_widget_bbox(self)
    else:
        if bpy.data.objects.find(self.treeface_widget_bbox) != -1:
            bbox_obj = bpy.data.objects[self.treeface_widget_bbox]
            bbox_obj.parent = None
            for scene in bbox_obj.users_scene:
                scene.objects.unlink(bbox_obj)
        self.treeface_widget_bbox = ""

def on_treeface_widget_width_update(self, context):
    print("update width:", self, context)
    if self.is_treeface_widget:
        repaint_widget_bbox(self)
        

def on_treeface_widget_height_update(self, context):
    if self.is_treeface_widget:
        repaint_widget_bbox(self)

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
                                                                 description = "object is root node of a treeface widget",
                                                                 update         = on_is_treeface_widget_update)
    
    bpy.types.Object.treeface_widget_bbox = bpy.props.StringProperty()

    bpy.types.Object.treeface_widget_type = bpy.props.EnumProperty(name    = "widget type",
                                                                   items   = [("TREEFACE_WIDGET_BASE",           "base",           "base type widget"),
                                                                              ("TREEFACE_WIDGET_SPINNER",        "spinner",        "widget that have a rotatable stuff"),
                                                                              ("TREEFACE_WIDGET_SLIDER",         "slider",         "widget that have a sliding stuff"),
                                                                              ("TREEFACE_WIDGET_BUTTON",         "button",         "common button"),
                                                                              ("TREEFACE_WIDGET_TRIGGER_BUTTON", "trigger button", "button that have two states: pushed and poped")],
                                                                   default = 'TREEFACE_WIDGET_BASE')
    
    bpy.types.Object.treeface_widget_width = bpy.props.IntProperty(name        = "widget width",
                                                                   description = "width of the widget",
                                                                   min         = 1,
                                                                   step        = 10,
                                                                   default     = 10,
                                                                   update         = on_treeface_widget_width_update,
                                                                   subtype     = 'PIXEL')
    
    bpy.types.Object.treeface_widget_height = bpy.props.IntProperty(name        = "widget height",
                                                                    description = "height of the widget",
                                                                    min         = 1,
                                                                    step        = 10,
                                                                    default     = 10,
                                                                    update         = on_treeface_widget_height_update,
                                                                    subtype     = 'PIXEL')

    bpy.types.Object.treeface_widget_custom = bpy.props.StringProperty(name = "custom data")

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

def unregister():
    bpy.utils.unregister_class(TreefaceObjectPanel)
    bpy.utils.unregister_class(TreefaceScenePanel)

if __name__ == "__main__":
    register()
