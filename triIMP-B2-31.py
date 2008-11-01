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
from Blender import sys
from Blender.Mathutils import *

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
		p = Blender.sys.dirname(self.filename)
		os.chdir(p)
		print "CWD is now %s" % (p)
		scene=Blender.Scene.GetCurrent()
		
		mesh = Blender.Mesh.New()
		
		words = lines[0].split()
		if len(words) <>1 :
			print "Not a raydium .tri file (first line must be 0 or 1)"
			return 0
		version = float(words[0])
		if version <0 or version >1:
			print "Not a supported (0 or 1) file format"
			return 0

		n = 0
		vi = 0
		fi = 0
		
		ntot = len(lines)-1
		naff = ntot /10
		ncour = 0
		
		print "Reading Datas"
		lfilename =[]
		faces = []
		verts =[]
		lvertn=[]
		
		vbuv=[]
		ltexname=[]
	
		for line in lines[1:]:
			ncour=ncour+1
			if (ncour % naff)==0:
				print (ncour*100/ntot)+1

			words = line.split()
			
			verts.append([float(words[0]),float(words[1]),float(words[2])])
			vi=vi+1
			
			if version==1:
				lvertn.append(Vector(float(words[3]),float(words[4]),float(words[5])))
				vbuv.append(Vector(float(words[6]),float(words[7])))
				ltexname.append(words[8])
			if version==0:
				vbuv.append(Vector(float(words[3]),float(words[4])))
				ltexname.append(words[5])
					
			if (vi%3)==0:
				faces.append([vi-3,vi-2,vi-1])
				fi=fi+1
				
		print "Done Read %d vertices %d faces" % (vi,fi)
		
		print "Creating Mesh"	
		mesh.verts.extend(verts)
		mesh.faces.extend(faces)
		print "Done"
		
		if version==1:
			print "Updating %d Normals" % vi
			for i in range(vi):
				mesh.verts[i].no = lvertn[i]
			print "Done."
		
		print "Updating %d Face UV" % fi
		
		limages=[]
		nimages=[]
		for i in range(fi):
			if (i % (fi/10))==0:
				print (i*100/fi)+1
			for j in range(3):
				ij = i*3+j
				face = mesh.faces[i]
				texname=ltexname[ij]
				if texname[0:4] == "rgb(":
					
					try:
						mesh.activeColorLayer = "rgb"
					except:
						print "Adding ColorLayer"
						mesh.addColorLayer("rgb")
						
					mesh.activeColorLayer = "rgb"
					
					temp=texname[4:-1].split(")")
					couls=temp[0].split(",")
					face.col[j].r=int(float(couls[0])*255)
					face.col[j].g=int(float(couls[1])*255)
					face.col[j].b=int( float(couls[2])*255)
					face.col[j].a=255
					
				else:
					filename = texname.split(";")
					for k in range(len(filename)):
						file = filename[k]
						param = file.split("|")
						if (len(param)==1):
							texname = param[0]
							if (k==0):
								buv = vbuv[ij]
							else:
								buv = []
						if (len(param)==3):
							texname = param[2]
							buv = [float(param[0]),float(param[1])]
						
						ntexname = "tex.%d" % (k)	
						try:
							mesh.activeUVLayer = ntexname
						except:
							print "Add UV Layer %s" % ntexname
							mesh.addUVLayer(ntexname)
							
						mesh.activeUVLayer = ntexname							
						
						found =-1	
						for l  in range(len(nimages)):
							if texname == nimages[l]:
								found=l
								break
							
						if (found==-1):	
							nimages.append(texname)							
							if fileExists(texname) == 0:
								print "Cannot access to %s" % (texname)	
								limages.append('no_tex')
							else:
								limages.append(Blender.Image.Load(texname))
								found = len(limages)-1
								print "Loading texture %s" % texname
								
								
						face.image = limages[found]
						face.mode |= Blender.Mesh.FaceModes['TEX']
						face.mode |= Blender.Mesh.FaceModes['TWOSIDE']

						if (len(buv)>0):
							face.uv[j][0] = buv[0];face.uv[j][1] = buv[1];

		mesh.activeUVLayer = "tex.0"
		print "Done"
		
		scn = Blender.Scene.GetCurrent()
		ob = scn.objects.new(mesh)
		ob.setLocation(0,0,0)
		Blender.Redraw()
		return vi		

	def close(self):
			#self.file.flush()
			self.file.close()

def importTRI(filename):
	t=sys.time()
	if filename.find('.tri', -4) <= 0: filename += '.tri'
	obj=source(filename)
	ret = obj.readFaces()
	obj.close
	if ret>0:
		print "Imported."
	else:
		Blender.Draw.PupMenu("Bad File type not imported See console.")
	print "Imported %d vertices in %.2fs" % (ret,sys.time()-t)

def filesel_callback(filename):
	test = importTRI(filename)

Blender.Window.FileSelector(filesel_callback, 'Import TRI','*.tri')
#importTRI("cocorobix.tri")