import sys
import os
import bpy
from bpy_extras.io_utils import ExportHelper

bl_info = {
    "name":        "treeface exporter",
    "author":      "Xi Yang",
    "location":    "File > Export",
    "description": "export scene to treeface JSON format",
    "category":    "Import-Export",
}

def format_matrix(mat):
    result_list = []
    for i in [0, 1, 2, 3]:
        for j in [0, 1, 2, 3]:
            result_list.append(mat[j][i])
    return result_list

def is_identity_matrix(mat):
    return (mat[0][0] == 1 and mat[0][1] == 0 and mat[0][2] == 0 and mat[0][3] == 0 and
            mat[1][0] == 0 and mat[1][1] == 1 and mat[1][2] == 0 and mat[1][3] == 0 and
            mat[2][0] == 0 and mat[2][1] == 0 and mat[2][2] == 1 and mat[2][3] == 0 and
            mat[3][0] == 0 and mat[3][1] == 0 and mat[3][2] == 0 and mat[3][3] == 1)

def get_scene_toplevel_objects(scene):
    toplevels = []
    for object in scene.objects:
        if not object.parent:
            toplevels.append(object)
    return toplevels

def collect_object_by_type(objects):
    nodes = []
    visuals = []
    for obj in objects:
        if obj.type == 'EMPTY' and obj.is_treeface_scene_node:
            nodes.append(obj)
        elif obj.type == 'MESH' and obj.is_treeface_visual_object:
            visuals.append(obj)
    return nodes, visuals

def export_geometry(mesh, dir):
    # determine whether we have some attributes
    normal_is_split = False
    has_uv = False
    has_tangent = False

    if mesh.use_auto_smooth:
        normal_is_split = True

    if len(mesh.uv_layers.items()) > 0:
        has_uv = True

    if normal_is_split and has_uv:
        has_tangent = True

    # build-in-calculate some attributes
    mesh.calc_normals()

    if normal_is_split:
        mesh.calc_normals_split(mesh.auto_smooth_angle)

    if has_tangent:
        mesh.calc_tangents()

    file_out = os.path.join(dir, "mesh_" + mesh.name + ".json")
    fh = open(file_out, 'w')
    fh.write("{\n")

    # write vertex attributes
    fh.write("    \"primitive\": \"triangle_fans\",\n")
    fh.write("    \"attributes\":\n")
    fh.write("    [\n")
    fh.write("        {\n")
    fh.write("            \"name\": \"position\",\n")
    fh.write("            \"n_elem\": 3,\n")
    fh.write("            \"type\": float,\n")
    fh.write("        },\n")
    fh.write("        {\n")
    fh.write("            \"name\": \"normal\",\n")
    fh.write("            \"n_elem\": 3,\n")
    fh.write("            \"type\": float,\n")
    fh.write("        },\n")

    if has_tangent:
        fh.write("        {\n")
        fh.write("            \"name\": \"tangent\",\n")
        fh.write("            \"n_elem\": 3,\n")
        fh.write("            \"type\": float,\n")
        fh.write("        },\n")

    if has_uv:
        fh.write("        {\n")
        fh.write("            \"name\": \"tex_coord\",\n")
        fh.write("            \"n_elem\": 2,\n")
        fh.write("            \"type\": float,\n")
        fh.write("        },\n")

    fh.write("    ],\n")

    # gather vertices
    vertices = []
    indices = []
    index_seed = 0

    for poly in mesh.polygons:
        for i_loop in range(poly.loop_start, poly.loop_start+poly.loop_total):
            data = []

            loop = mesh.loops[i_loop]
            vtx = mesh.vertices[loop.vertex_index]

            # position
            data.extend(vtx.co)

            # normal
            if normal_is_split:
                data.extend(loop.normal)
            else:
                data.extend(vtx.normal)

            # tangent
            if has_tangent:
                data.extend(loop.tangent)

            # uv
            if has_uv:
                data.extend(mesh.uv_layers.active.data[i_loop].uv)

            indices.append(index_seed)
            vertices.append(data)
            index_seed += 1

        indices.append(65535)

    # write vertices
    fh.write("    \"vertices\":\n");
    fh.write("    [\n");
    for vtx_data in vertices:
        fh.write("        [ " + ', '.join(map(str,vtx_data)) + " ],\n");
    fh.write("    ],\n");

    # write indices
    fh.write("    \"indices\":\n");
    fh.write("    [\n");
    for idx in indices:
        fh.write("        " + str(idx) + ",\n");
    fh.write("    ],\n");

    # finalize
    fh.write("}\n")
    fh.close();
    return True;

def write_visual_object(object, fh, indent):
    fh.write(' ' * (indent) + "{\n")
    fh.write(' ' * (indent + 4) + "\"geometry\": \"mesh_" + object.data.name + ".json\",\n")
    if object.active_material > 0:
        fh.write(' ' * (indent + 4) + "\"material\": \"material_" + object.active_material.name + ".json\",\n")
    else:
        fh.write(' ' * (indent + 4) + "\"material\": \"material_DEFAULT.json\",\n")
    fh.write(' ' * (indent) + "},\n")

def write_node_recur(object, fh, indent):
    # collect visual objects and child nodes
    child_nodes, visuals = collect_object_by_type(object.children)

    # write head
    fh.write(' ' * indent + "{\n")
    fh.write(' ' * (indent + 4) + "\"id\": \"" + object.name + "\",\n")
    fh.write(' ' * (indent + 4) + "\"transform\": [ " + ', '.join(format_matrix(object.matrix_local)) + " ],\n")

    # write visual items
    fh.write(' ' * (indent + 4) + "\"visual\":\n")
    fh.write(' ' * (indent + 4) + "[\n")

    for visual_obj in visuals:
        write_visual_object(visual_obj, fh, indent + 4)

    fh.write(' ' * (indent + 4) + "],\n")

    # write children
    fh.write(' ' * (indent + 4) + "\"children\":\n")
    fh.write(' ' * (indent + 4) + "[\n")

    for child in child_nodes:
        export_node_recur(child, fh, indent + 4)

    fh.write(' ' * (indent + 4) + "],\n")

    # finalize
    fh.write(' ' * indent + "}\n")

def write_global_light(light, fh, indent):
    fh.write(' ' * indent + "\"global_light_direction\": [" +  + "],\n")
    fh.write(' ' * indent + "\"global_light_position\": [" +  + "],\n")
    fh.write(' ' * indent + "\"global_light_intensity\": [" +  + "],\n")

def write_widget_recur(object, fh, indent):
    pos_x = object.matrix_local[0][3] - object.treeface_widget_width / 2
    pos_y = object.matrix_local[1][3] - object.treeface_widget_height / 2 
    
    fh.write(' ' * indent + "{\n")
    fh.write(' ' * (indent + 4) + "\"id\": \"" + object.name + "\",\n")
    fh.write(' ' * (indent + 4) + "\"type\": \"" + object.treeface_widget_type + "\",\n")
    fh.write(' ' * (indent + 4) + "\"position\": [ " + str(pos_x) + ', ' + str(pos_y) + " ],\n")
    fh.write(' ' * (indent + 4) + "\"size\": [ " + str(object.treeface_widget_width) + ', ' + str(object.treeface_widget_height) + " ],\n")
    fh.write(' ' * (indent + 4) + "\"node\": \"" + object.name + "\",\n")
    
    # TODO object custom nodes
    
    fh.write(' ' * indent + "},\n")

def export_scene(scene, dir):
    scene_toplevel_objects = get_scene_toplevel_objects(scene)
    nodes, visuals = collect_object_by_type(scene_toplevel_objects)
    
    file_scene = os.path.join(dir, "scene_" + scene.name + ".json")
    fh = open(file_scene, 'w')
    fh.write("{\n")
    
    write_global_light(bpy.data.lamps.get(scene.treeface_global_light), fh, 4)
    
    fh.write("    \"node\":\n")
    fh.write("    {\n")
    fh.write("        \"transform\": [ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 ],\n")
    
    # write visual objects at root
    fh.write("        \"visual\":\n")
    fh.write("        [\n")
    
    for visual in visuals:
        write_visual_object(visual, fh, 12)
    
    fh.write("        ],\n")
    
    # write nodes
    fh.write("        \"children\":\n")
    fh.write("        [\n")
    
    for node in nodes:
        write_node_recur(node, fh, 12)
    
    fh.write("        ],\n")
    fh.write("    },\n")
    
    # finalize
    fh.write("}\n")
    fh.close()

def export_widget(scene, dir):
    file_widget = os.path.join(dir, "scene_widget_" + scene.name + ".json")
    fh = open(file_widget, 'w')
    fh.write("{\n")
    
    fh.write("    \"position\": [0, 0],")
    
    fh.write("}\n")
    fh.close()

class TreefaceExporter(bpy.types.Operator, ExportHelper):
    """Export the scene to treeface JSON file"""
    bl_idname = "treeface.export"
    bl_label = "Export as treeface to folder"
    filename_ext = "."
    use_filter_folder = True

    def execute(self, context):
        filepath = self.filepath
        if not os.path.isdir(filepath):
            self.report({'WARNING'}, "Path " + filepath + " is not a directory\n")
            return {'CANCELLED'}
        
        # validate scene
        # get meshes and materials
        project_ok, used_meshs, used_materials = validate_everything()
        if not project_ok:
            return {'CANCELLED'}
        
        # do export
        for mesh in used_meshes:
            export_geometry(mesh, filepath)

        for mat in used_materials:
            export_material(mat, filepath)

        export_scene(bpy.data.scenes[0], filepath)
        
        export_widget(bpy.data.scenes[0], filepath)

    def validate_everything(self):
        all_valid = True
        used_mesh_names = set()
        used_material_names = set()
        
        # we should have only one scene
        if len(bpy.data.scenes) == 0:
            self.report({'ERROR_INVALID_INPUT'}, "no scene in current project")
            all_valid = False
        
        if len(bpy.data.scenes) > 1:
            self.report({'ERROR_INVALID_INPUT'}, "more than one scene in current project")
            all_valid = False
        
        # validate visual objects
        # collect meshes
        for object in bpy.data.scenes[0].objects:
            if object.type == 'MESH' and object.is_treeface_visual_object:
                if not is_identity_matrix(object.matrix_local):
                    self.report({'ERROR_INVALID_INPUT'}, "visual object " + object.name + " local matrix is not identity")
                    all_valid = False
                
                if len(object.children) > 0:
                    self.report({'ERROR_INVALID_INPUT'}, "visual object " + object.name + " has child objects, which is not wanted")
                    all_valid = False
                    
                used_mesh_names.add(object.data.name)
                if object.active_material:
                    used_material_names.add(object.active_material.name)
                    
                # visual object's parent must be a scene node, or has no parent
                if object.parent:
                    if not object.parent.type == 'EMPTY' or not object.parent.is_treeface_scene_node:
                        self.report({'ERROR_INVALID_INPUT'}, "visual object " + object.name + "'s parent " + object.parent.name + " is not a scene node")
                        all_valid = False
                
            elif object.type == 'EMPTY' and object.is_treeface_scene_node:
                if object.parent:
                    if not object.parent.type == 'EMPTY' or not object.parent.is_treeface_scene_node:
                        self.report({'ERROR_INVALID_INPUT'}, "scene node object " + object.name + "'s parent " + object.parent.name + " is not a scene node")
                        all_valid = False
                    
                    if object.is_treeface_widget:
                        if not object.parent.type == 'EMPTY' or not object.parent.is_treeface_scene_node or not object.parent.is_treeface_widget:
                            self.report({'ERROR_INVALID_INPUT'}, "widget object " + object.name + "'s parent " + object.parent.name + " is not a widget object")
                            all_valid = False
    
                if object.is_treeface_widget:
                    if not object.is_treeface_scene_node:
                        self.report({'ERROR_INVALID_INPUT'}, "widget object " + object.name + " is not a scene node object")
                        all_valid = False
                    
        # validate meshes
        used_meshes= []
        for mesh in bpy.data.meshes:
            if not mesh.name in used_mesh_names:
                continue
            if len(mesh.materials) > 1:
                self.report({'ERROR_INVALID_INPUT'}, "mesh " + mesh.name + " has more than one materials")
                all_valid = False
                
            if len(mesh.uv_layers) > 1:
                self.report({'ERROR_INVALID_INPUT'}, "mesh " + mesh.name + " has more than one UV layers")
                all_valid = False
                
            used_meshes.append(mesh)
            
        # validate materials
        used_materials = []
        for mat in bpy.data.materials:
            if not mat.name in used_material_names:
                continue
            used_materials.append(mat)
    
        return all_valid, used_meshes, used_materials

def menu_func(self, context):
    self.layout.operator(TreefaceExporter.bl_idname)

def register():
    bpy.utils.register_class(TreefaceExporter)
    bpy.types.INFO_MT_file_export.append(menu_func)
    #bpy.types.VIEW3D_PT_tools_objectmode.prepend(menu_func)

def unregister():
    bpy.utils.unregister_class(TreefaceExporter)
    bpy.types.INFO_MT_file_export.remove(menu_func)
    #bpy.types.VIEW3D_PT_tools_objectmode.remove(menu_func)

if __name__ == "__main__":
    register()
