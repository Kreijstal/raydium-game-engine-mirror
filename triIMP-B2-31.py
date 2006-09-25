#!BPY

"""
Name: 'Raydium Import (.tri format)'
blender: 2.31
Group: 'Import'
Tooltip: 'Import a .tri'
"""
#import rpdb2;
#rpdb2.start_embedded_debugger("test",True)

try:
  import nt
  os=nt
  os.sep='\\'
except:
  import posix
  os=posix
  os.sep='/'

import sys
import Blender
from Blender import NMesh

# This script will include .tri files (Raydium mesh files)
# Do not supports multitexturing yet.
# Python puxxor :)

def fileExists(f):
     try:
        file = open(f)
     except IOError:
         exists = 0
     else:
         exists = 1
     return exists

class source:
	def __init__(self,filename):
		self.filename=filename
		self.file=open(filename,"r")

	def readFaces(self):
		lines = self.file.readlines();
		#p,f = os.path.split(self.filename)
		#p,f = Blender.sys.splitext(self.filename)
		p = Blender.sys.dirname(self.filename)
		#f = Blender.sys.basename(self.filename)
		os.chdir(p)
		print "CWD is now %s" % (p)
		scene=Blender.Scene.getCurrent()
		mesh = Blender.NMesh.New()
		
		n = 0
		bvert = [0, 0, 0]
		bu = [0, 0, 0]
		bv = [0, 0, 0]
		textures = []
		textures_index = []

		for line in lines:			
		    words = line.split()
		    if len(words) == 1:
			if float(words[0]) != 1.0:
			    print "ERROR ! must be a .tri version 1 ! use raydium_modler/ Trying to read."
			    version = 0
			else:
			     print "Fichier version 1 Ok!"
			     version = 1
		    else:
		         if version==1:
        			vx = float(words[0])
        			vy = float(words[1])
        			vz = float(words[2])
        			nx = float(words[3])
        			ny = float(words[4])
        			nz = float(words[5])
        			bu[n] = float(words[6])
        			bv[n] = float(words[7])
        			vert=Blender.NMesh.Vert(vx,vy,vz)
        			bvert[n]=vert
                        	mesh.verts.append(vert)
        			n = n + 1
        			texname=words[8]
        		 if version==0:
        			vx = float(words[0])
        			vy = float(words[1])
        			vz = float(words[2])
        			bu[n] = float(words[3])
        			bv[n] = float(words[4])
        			vert=Blender.NMesh.Vert(vx,vy,vz)
        			bvert[n]=vert
                        	mesh.verts.append(vert)
        			n = n + 1
                                texname=words[5]
#        		 print texname
		    if n == 3:
			textured = 1
			if texname[0:4] == "rgb(":
                            temp=texname[4:-1].split(")")
                            texname=temp[0]
                            print texname
                            textured = 0
                            couls=texname.split(",")
                            r= float(couls[0])*255
                            g= float(couls[1])*255
                            b= float(couls[2])*255

			bface = Blender.NMesh.Face()
			#bface.mode |= Blender.NMesh.FaceModes['TWOSIDE']

			if textured == 1:
    			    bface.mode |= Blender.NMesh.FaceModes['TEX']
			bface.materialIndex=0
			bface.v.append(bvert[0])
			bface.v.append(bvert[1])
			bface.v.append(bvert[2])
			bface.uv.append( (bu[0], bv[0]) )
			bface.uv.append( (bu[1], bv[1]) )
			bface.uv.append( (bu[2], bv[2]) )

			if textured ==0:
              		   bface.mode &= ~Blender.NMesh.FaceModes['TEX']
                           for n in range(3):
                             col =NMesh.Col()
                             col.r=int(r)
                             col.g=int(g)
                             col.b=int(b)
                             col.a=255
                             bface.col.append(col)

			if textured == 1:
			    found = -1
			    filename = texname.split(";")
			    texname=filename[0]
			    for t in range(len(textures)):
				if textures[t] == texname:
				    found = textures_index[t]

			    if found == -1:
				print "texture (%s)is not cached yet: loading" % (texname)
				if fileExists(texname) == 0:
				    print "Cannot access to %s" % (texname)
				    #sys.exit(3)
				found = Blender.Image.Load(texname)
				textures.append(texname)
				textures_index.append(found)
			    
			    bface.image = found
			mesh.faces.append(bface)
			n = 0

		object = Blender.NMesh.PutRaw(mesh)
		object.name = "tri"
		print "end."
		

	def close(self):
			#self.file.flush()
			self.file.close()

def importTRI(filename):
    if filename.find('.tri', -4) <= 0: filename += '.tri'
    obj=source(filename)
    obj.readFaces()
    obj.close
    print "Imported."

def filesel_callback(filename):
  test = importTRI(filename)

Blender.Window.FileSelector(filesel_callback, "Import TRI")
#importTRI("exported v0.tri")
