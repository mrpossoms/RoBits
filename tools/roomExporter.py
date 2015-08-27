import struct

def exportRoom(path):
fp   = open(path, "wb")
mesh = D.objects[0].to_mesh(C.scene, True, 'PREVIEW')
verts = mesh.vertices

# write the number of edges
fp.write(struct.pack('I', int(len(mesh.edges))))
print('Writing ' + str(len(mesh.edges)))

# iterate over each edge
for edge in mesh.edges:
	for vi in edge.vertices:
		# write each vertex in each edge
		v = verts[vi].co
		fp.write(struct.pack('ffff', v[0], v[1], v[2], 1))
	# write the color
	fp.write(struct.pack('ffff', 1, 0, 1, 1))

fp.close()