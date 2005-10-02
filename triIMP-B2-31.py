#!BPY

"""
Name: 'Raydium Import (.tri format)'
blender: 2.31
Group: 'Import'
Tooltip: 'Import a .tri'
"""

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
			    print "ERROR ! must be a .tri version 1 ! use raydium_modler."
		    else:
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

		    if n == 3:
			textured = 1
			if words[8][0:4] == "rgb(":
			    textured = 0
			    #print "Raydium rgb() material found.. ignoring !"
			#else:
			    #print "textured face"
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
			if textured == 1:
			    found = -1
			    for t in range(len(textures)):
				if textures[t] == words[8]:
				    found = textures_index[t]
			
			    if found == -1:
				print "texture (%s)is not cached yet: loading" % (words[8])
				if fileExists(words[8]) == 0:
				    print "Cannot access to %s" % (words[8])
				    sys.exit(3)
				found = Blender.Image.Load(words[8])
				textures.append(words[8])
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
    obj=source(filename)
    obj.readFaces()
    obj.close
    print "Imported."

def filesel_callback(filename):
  test = importTRI(filename)
  
Blender.Window.FileSelector(filesel_callback, "Import TRI")
  
