import bpy
import os
import time

#bpy.data.meshes.remove(my_mesh)
#bpy.ops.object.convert(target='MESH',keep_original=True)

active_object=None
selected_objects=[]
active_object_mode=''
temp_object=None
all_scene=True

def save_state(context):
    global active_object,selected_objects,active_object_mode
    
    print ("Saving State")
    
    active_object=context.active_object
    if active_object!=None:
        active_object_mode = context.object.mode
        #Switch to object mode as default
        bpy.ops.object.mode_set(mode='OBJECT')
    
    selected_objects = context.selected_objects
    if len(selected_objects)==0:
        bpy.ops.object.select_all()
        selected_objects = context.selected_objects      
        print ("No object selected, saving all objects.")
    

def restore_state(context):
    global selected_objects,active_object,active_object_mode
    
    print ("Restoring State.")

    #Switch to object mode
    ob=context.object
    if ob!=None:
        if ob.mode=='EDIT':
            bpy.ops.object.mode_set(mode='OBJECT')
            
    if active_object!=None:
        bpy.ops.object.select_name(name=active_object.name)

    bpy.ops.object.select_all(action='DESELECT')
    for ob in selected_objects:
        ob.select=True  

    if active_object_mode=='EDIT':
        bpy.ops.object.mode_set(mode='EDIT')        

def convert_to_mesh(obj):
    global temp_object
    
    print ("Creating temporary working Mesh for {}.".format(obj))
    
    if obj.type=='MESH':
        bpy.ops.object.duplicate()
    else:
        bpy.ops.object.convert(target='MESH',keep_original=True)    
        
    temp_object=bpy.context.object
    
    return temp_object

def convert_quad_to_tri(obj):
    print ("Triangulate Faces")
    bpy.ops.object.select_name(name=obj.name)
    bpy.ops.object.mode_set(mode='EDIT')
    bpy.ops.mesh.select_all(action='SELECT')
    bpy.ops.mesh.quads_convert_to_tris()
    bpy.ops.object.mode_set(mode='OBJECT')

def export_vertice(vert,face,me,obj,f):    
    global all_scene
    
    if all_scene:
        rot = obj.matrix_world
        pos = obj.location
        vloc,qrot,vscale = rot.decompose()    
    
    v=me.vertices[vert].co.copy()
    
    if face.use_smooth:
        n=me.vertices[vert].normal.copy()
    else:
        n=face.normal.copy()
    
    if all_scene:
        v = (v * rot)
        n = n * qrot

    f.write("%+0f %+0f %+0f"%(v[0],v[1],v[2]))
    f.write("  %+0f %+0f %+0f"%(n[0],n[1],n[2]))

def build_vertex_color_list(i_vert,i_face,me):
    tex_list=[]
    for vcol in me.vertex_colors:
        if i_vert==0:
            col = vcol.data[i_face].color1
        if i_vert==1:
            col = vcol.data[i_face].color2
        if i_vert==2:
            col = vcol.data[i_face].color3
        r=col[0]
        g=col[1]
        b=col[2]
        texture=("rgb(%3.2f,%3.2f,%3.2f)"%(r,g,b))
        tex_list.append({'u':0.0,'v':0.0,'tex':texture})        

    return tex_list

def build_uv_texture_list(i_vert,i_face,me,obj):
    tex_list=[]
    for uvt in me.uv_textures:            
        if uvt.data[i_face].use_image:
            if i_vert==0:
                uv=uvt.data[i_face].uv1
            if i_vert==1:
                uv=uvt.data[i_face].uv2
            if i_vert==2:
                uv=uvt.data[i_face].uv3
            texture=os.path.basename(uvt.data[i_face].image.filepath)
            if not(os.path.isfile(texture)) or uvt.data[i_face].image.file_format!='TARGA':
                image=uvt.data[i_face].image
                image.file_format='TARGA'
                image.filepath_raw=os.path.basename(image.filepath).split('.')[0]+'.tga'
                image.save()
                texture=os.path.basename(uvt.data[i_face].image.filepath)
                
            tex_list.append({'u':uv[0],'v':uv[1],'tex':texture})  

        if uvt.data[i_face].use_object_color:
            r=obj.color[0]
            g=obj.color[1]
            b=obj.color[2]
            texture=("rgb(%3.2f,%3.2f,%3.2f)"%(r,g,b))
            tex_list.append({'u':0.0,'v':0.0,'tex':texture})

    return tex_list
            
        
def export_mesh(obj,f):
    me=obj.data
    #me=obj.to_mesh(scene=bpy.context.scene,apply_modifiers=True,settings='RENDER')
    
    nf=len(me.faces)
    lper=-11
    print ("Exporting mesh %s, %d faces, %d vertices"%(obj.data.name,len(me.faces),len(me.vertices)))
    for i_face,face in enumerate(me.faces):

        per=i_face/nf*100
        if per>lper+10:
            print("%d%% (%d)"%(per,i_face))
            lper=per
        #print (i_face)
        verts = face.vertices
        
        for i_vert,vert in enumerate(verts):
            export_vertice(vert,face,me,obj,f)

            tex_list=build_uv_texture_list(i_vert,i_face,me,obj)
            tex_list+=build_vertex_color_list(i_vert,i_face,me)

            #print ("**"+tex_list)
            
            if len(tex_list)==0:
                tex_list.append({'u':0.0,'v':0.0,'tex':'rgb(0.6,0.6,0.6)'})
                
            tex=tex_list[0]
            f.write("  %f %f %s"%(tex['u'],tex['v'],tex['tex']))
            tex_list.remove(tex)
            for tex in tex_list:
                f.write(";%f|%f|%s"%(tex['u'],tex['v'],tex['tex']))                                       

            f.write("\n")
    #bpy.data.meshes.remove(me)    

def write_some_data(context, filepath, save_elements):
    global selected_objects,all_scene

    print ("Using file name: %s Working directory: %s"%(os.path.basename(filepath),os.path.dirname(filepath)))
    
    print ("Changing working directory to %s"%os.path.dirname(filepath))
    os.chdir(os.path.dirname(filepath))
   
    if save_elements:
        print ("Saving Each element in its own file according to mesh name.")
        all_scene=False
    else:
        print ("Saving all scene in one file.")
        print ("Opening %s for writing."%os.path.basename(filepath))
        f =open(os.path.basename(filepath),'w')
        print ("Version 1 tri file")
        f.write("1\n")
        all_scene=True
    
    save_state(context)

    print ("%d objects to export."%(len(selected_objects)))                           
    for obj in selected_objects:
 
        if obj.type=='LAMP' or obj.type=='CAMERA':
            print ("Skipping object {}.".format(obj))
            continue

        bpy.ops.object.select_all(action='DESELECT')
        bpy.ops.object.select_name(name=obj.name)

        print ("Exporting {} / {} Type:{}".format(obj,obj.name,obj.type))
           
        wobj = convert_to_mesh(obj)
        if not all_scene:
            file_name=wobj.data.name
            file_name+='.tri'
            print ("Opening %s for writing."%file_name)
            f =open(file_name,'w')
            print ("Version 1 tri file")
            f.write("1\n")               
            
        convert_quad_to_tri(wobj)
        export_mesh(wobj,f)
            
        print ("Removing temporary object.")
        bpy.ops.object.delete()

        if not all_scene:
            print("Closing %s file."%file_name)
            f.close()

    print ("Closing tri file.")
    if all_scene:
        f.close()
    restore_state(context)
    return {'FINISHED'}


def read_some_data(context,filepath,clean_mesh):
    
    print ("Using file name: %s Working directory: %s"%(os.path.basename(filepath),os.path.dirname(filepath)))
    
    print ("Changing working directory to %s"%os.path.dirname(filepath))
    os.chdir(os.path.dirname(filepath))
    print ("importing %s."%filepath)
    
    finf=os.stat(filepath)
    fsiz=finf.st_size
    file=open(filepath,'r')
    name=os.path.basename(filepath).split('.')[0]
    ver = file.readline()
    print (ver)
    ver=int(ver)
    if ver != 1:
        print ("Not a version 1 tri file. Not supported.")
        return {'CANCELED'}
    print ("go")

            
 
    print ('Creating Object')
    bpy.ops.object.add(type='MESH')
    #print ('Creating Mesh %s'%name)
    #bpy.data.meshes.new(name)
    #me=bpy.data.meshes[name]
    #bpy.context.object.data=me
    me=bpy.context.object.data
    print ("Reading datas.")

    nv=0
    ne=0
    nf=0

    v=me.vertices
    e=me.edges
    f=me.faces
    
    n=0
    while 1:
        vert=file.readline() 
        n=n+1
        if not vert:
            break
        
    file.seek(0)    #rewind file
    file.readline() #skip first line (version)
    
    #allocate vertices,edges face, very very faster this way 
    v.add(n)
    f.add(n/3)
    e.add(n)

    lper=-11    
    while 1:
        #print ("Vertices:%d Edges:%d Faces:%d"%(nv,ne,nf))
        vert=file.readline() 
        
        if not vert:
            break


        per=nv/n*100;
        if per>lper+10:
            print ("%d%% (%d)"%(per,nf))
            lper=per
        
        vs=vert.split()

        v[nv].co=[float(vs[0]),float(vs[1]),float(vs[2])]

        if ver==1:
            v[nv].normal=[float(vs[3]),float(vs[4]),float(vs[5])]

        #now compute texture information
        tu=[]
        tv=[]
        tt=[]
        tu.append(float(vs[6]))
        tv.append(float(vs[7]))
        
        t=vs[8] #full texture name
        
        t=t.split(';') #split textures name, first name only, others u|v|name
        
        tt.append(t[0]) #add first texture name to texture list
        t.remove(t[0])
        
        for it  in t:
            #print (it)
            iit=it.split('|')
            tu.append(float(iit[0]))
            tv.append(float(iit[1]))
            tt.append(iit[2])
        
        ntt=0 # number of texture layers
        ntc=0 # number of vertex color layers
        for i,t in enumerate(tt): #for each texture
            if t.find('rgb')==0:
                ntc=ntc+1
                if ntc > len(me.vertex_colors):
                    print ("Adding RGB texture layer")
                    me.vertex_colors.new()
                l=t.split('(')
                l=l[1]
                l=l.split(',')
                l[2]=l[2].split(')')[0]
                if (nv%3)==0:
                    me.vertex_colors[ntc-1].data[nf].color1=[float(l[0]),float(l[1]),float(l[2])]
                if (nv%3)==1:
                    me.vertex_colors[ntc-1].data[nf].color2=[float(l[0]),float(l[1]),float(l[2])]
                if (nv%3)==2:
                    me.vertex_colors[ntc-1].data[nf].color3=[float(l[0]),float(l[1]),float(l[2])]                                        
            else:
                ntt=ntt+1
                #print (t)
                if ntt > len (me.uv_textures):
                    print ("Adding uv texture layer")
                    me.uv_textures.new()
                im=bpy.data.images.get(t)
                if not(isinstance(im,bpy.types.Image)):
                    print ("Loading image %s.",t)
                    bpy.data.images.load(t)
                im=bpy.data.images.get(t)                        
                me.uv_textures[ntt-1].data[nf].image=im
                me.uv_textures[ntt-1].data[nf].use_image=True
                if(nv%3)==0:
                    me.uv_textures[ntt-1].data[nf].uv1=[tu[i],tv[i]]
                if(nv%3)==1:
                    me.uv_textures[ntt-1].data[nf].uv2=[tu[i],tv[i]]
                if(nv%3)==2:
                    me.uv_textures[ntt-1].data[nf].uv3=[tu[i],tv[i]]
                    
                           

        nv=nv+1

        if (nv%3)==0:
            e[ne].vertices=[(nv-3),(nv-2)]
            e[ne+1].vertices=[(nv-2),(nv-1)]
            e[ne+2].vertices=[(nv-1),(nv-3)]
            ne=ne+3
            
            f[nf].vertices=[(nv-3),(nv-2),(nv-1)]
            
            if v[nv-3].normal==v[nv-2].normal and v[nv-2].normal==v[nv-1].normal:
                f[nf].use_smooth=False
            else:
                f[nf].use_smooth=True
                
            nf=nf+1                

    print ("Mesh Imported Vertices:%d Edges:%d Faces:%d."%(len(v),len(e),len(f)))
    #me.from_pydata(v,e,f)
    bpy.ops.object.mode_set(mode='EDIT')
    print("close file.")
    file.close()  
    if clean_mesh:  
        bpy.ops.mesh.remove_doubles(limit=0.0001)
        bpy.ops.mesh.tris_convert_to_quads()
    bpy.ops.object.mode_set(mode='OBJECT')
    return {'FINISHED'} 

# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper,ImportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty


class ExportSomeData(bpy.types.Operator, ExportHelper):
    '''Export Raydium .tri file.'''
    bl_idname = "export.raydium_tri"  # this is important since its how bpy.ops.export.some_data is constructed
    bl_label = "Export Raydium Tri File"

    # ExportHelper mixin class uses this
    filename_ext = ".tri"

    filter_glob = StringProperty(default="*.tri", options={'HIDDEN'})

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.
    save_elements = BoolProperty(name="Save Each Element", description="Save each object as one file, using mesh name", default=False)

#    type = EnumProperty(items=(('OPT_A', "First Option", "Description one"),
#                               ('OPT_B', "Second Option", "Description two."),
#                               ),
#                        name="Example Enum",
#                        description="Choose between two items",
#                        default='OPT_A')
#
    @classmethod
    def poll(cls, context):
        #return context.active_object != None
        return len(context.selectable_objects)>0

    def execute(self, context):
        now=time.time()
        ret=write_some_data(context, self.filepath, self.save_elements)
        print ("Elapsed: %f"%(time.time()-now))        
        return ret
    
class ImportSomeData(bpy.types.Operator,ImportHelper):
    '''Import Raydium .tri file.'''
    bl_idname = "import_mesh.raydium_tri"  # this is important since its how bpy.ops.export.some_data is constructed
    bl_label = "Import Raydium Tri File"

    # ExportHelper mixin class uses this
    filename_ext = ".tri"

    filter_glob = StringProperty(default="*.tri", options={'HIDDEN'})

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.
    clean_mesh = BoolProperty(name="Clean Up Mesh", description="Remove doubles vertices and try to make quads faces", default=False)

#    type = EnumProperty(items=(('OPT_A', "First Option", "Description one"),
#                               ('OPT_B', "Second Option", "Description two."),
#                               ),
#                        name="Example Enum",
#                        description="Choose between two items",
#                        default='OPT_A')
#
    @classmethod
    def poll(cls, context):
        return True
        #return context.active_object != None
        #return len(context.selectable_objects)>0

    def execute(self, context):
        now=time.time()
        ret=read_some_data(context,self.filepath,self.clean_mesh) 
        print ("Elapsed: %f"%(time.time()-now))
        return ret


# Only needed if you want to add into a dynamic menu
def menu_func_export(self, context):
    self.layout.operator(ExportSomeData.bl_idname, text="Raydium (.tri)")

def menu_func_import(self, context):
    self.layout.operator(ImportSomeData.bl_idname, text="Raydium (.tri)")

def register():
    bpy.utils.register_class(ExportSomeData)
    bpy.types.INFO_MT_file_export.append(menu_func_export)
    bpy.utils.register_class(ImportSomeData)
    bpy.types.INFO_MT_file_import.append(menu_func_import)    


def unregister():
    bpy.utils.unregister_class(ExportSomeData)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)
    bpy.utils.unregister_class(ImportSomeData)
    bpy.types.INFO_MT_file_export.remove(menu_func_Import)    


if __name__ == "__main__":
    #unregister()
    try:
        unregister()
    except:
        pass
    register()

    # test call
    for i in range(20):
        print ("\n")
    
    #bpy.ops.export.raydium_tri('INVOKE_DEFAULT')
    #bpy.ops.import_mesh.raydium_tri('INVOKE_DEFAULT')

    try:
        bpy.ops.object.mode_set(mode='OBJECT')
    except:
        pass
    print ('remove object')
    bpy.ops.object.select_all(action='SELECT')
    bpy.ops.object.delete()
    print ('removed')
    
    try:
        while 1:
            print ('remove mesh')
            bpy.data.meshes.remove(bpy.data.meshes[0])
            print ('removed')
    except:
        pass    
    
    #read_some_data(bpy.context,'C:\\raydium_win32_sdk_1041\\bin\\Cube.smooth.053.tri')
    now=time.time()   
    read_some_data(bpy.context,'C:\\raydium_win32_sdk_1041\\bin\\a.tri',True)
    print ("Elapsed: %f."%(time.time()-now))
