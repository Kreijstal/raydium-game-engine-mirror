#!BPY

"""
Name: 'Raydium Export (.tri format)'
Blender: 2.36"
Group: 'Export'
Tooltip: 'Export to .tri format with normals'
"""

#import rpdb2;
#rpdb2.start_embedded_debugger("test",True)

import Blender

from Blender import NMesh

class source:
	def __init__(self,filename):
		self.file=open(filename,"w")

	def writeFaces(self):
		scene=Blender.Scene.getCurrent()
		self.file.write("1\n") #tri version
		
		for object in scene.getChildren():
		    objtype=object.getType()
		    if objtype == "Mesh":
			print "ok, it's a mesh"
			mesh=object.getData()
			vlist=mesh.verts
			for face in mesh.faces:
				if len(face)!=3:
				    print "ERROR: NOT A TRIANGLE ONLY MESH ! (select all vertices and use CTRL+T)"
		                for i in range(3): #triangles only ! (CTRL+T)
		                    indx=vlist.index(face[i])
				    if(face.image):
					u=face.uv[i][0]
					v=face.uv[i][1]
					self.file.write("%f %f %f %f %f %f %f %f %s\n" % (vlist[indx].co[0],vlist[indx].co[1],vlist[indx].co[2],vlist[indx].no[0],vlist[indx].no[1],vlist[indx].no[2],u,v,face.image.name))
				    else:
						if(len(face.col)==0):
							self.file.write("%f %f %f %f %f %f 0 0 rgb(1,1,0)\n" % (vlist[indx][0],vlist[indx][1],vlist[indx][2],vlist[indx].no[0],vlist[indx].no[1],vlist[indx].no[2]))
						else:
							self.file.write("%f %f %f %f %f %f 0 0 rgb(%3.3f,%3.3f,%3.3f)\n" % (vlist[indx][0],vlist[indx][1],vlist[indx][2],vlist[indx].no[0],vlist[indx].no[1],vlist[indx].no[2],face.col[i].r/255.0,face.col[i].g/255.0,face.col[i].b/255.0))
					
# get the face normals
#			self.file.write("GLfloat normals[%d][3] = {\n" % len(mesh.normals) )
#			for normal in mesh.normals:
#				self.file.write("{ %ff, %ff, %ff },\n" % (normal[0],normal[1],normal[2]))
#			self.file.write("};\n\n")

# get the mesh colors
#			if mesh.colors:
#				self.file.write("char OBJ_HAVE_COLORS=1;\n")
#				self.file.write("GLfloat colors[%d][3] = {\n" % len(mesh.colors) )
#				for color in mesh.colors:
#					self.file.write("{ %ff, %ff, %ff },\n" % (color[0],color[1],color[2]))
#				self.file.write("};\n\n")
#			else:
#				self.file.write("char OBJ_HAVE_COLORS=0;\nGLfloat *colors;\n")


	def close(self):
			self.file.flush()
			self.file.close()

def fs_callback(filename):
	if filename.find('.tri', -4) <= 0: filename += '.tri'
	obj=source(filename)
	obj.writeFaces()
	obj.close
	print "Exported to %s. Textures must be .tga, uncompressed, origin NOT at lower-left.\n" % (filename)

#fs_callback("tt.tri")
Blender.Window.FileSelector(fs_callback, "Export Raydium Tri")