#!/usr/bin/env python

import rospy

class map:

    def __init__(self, x, y, z, d, diction):
        self.px = x
        self.py = y
        self.pz = z
        self.dis = d
        self.diction = diction

    def cut(self, filein):
        
        file = self.diction + filein
        file_array = [self.diction + 'c.pcd', self.diction + 'f.pcd', self.diction + 'b.pcd', self.diction + 'l.pcd', self.diction + 'r.pcd']
        
        for fileout in file_array:
            current = 0
            dx = dy = 0
            if file_array.index(fileout) == 1:
                dx = 2 * self.dis
            if file_array.index(fileout) == 2:
                dx = -2 * self.dis
            if file_array.index(fileout) == 3:
                dy = 2 * self.dis
            if file_array.index(fileout) == 4:
                dy = -2 * self.dis                                               
            with open(file, 'r') as f:
                with open(fileout, 'w') as w:
                    line = f.readlines()
                    for i in range(11):
                        w.write(line[i])
                    for i in range(11,len(line)-1):
                        a, b, c, d, e = line[i].split(' ',4)
                        x = float(a)
                        y = float(b)
                        z = float(c) + 1.25
                        intensity = int(d)
                        ring = int(e)

                        if x + dx > self.px - self.dis and x + dx < self.px + self.dis and y + dy > self.py - self.dis and y + dy < self.py + self.dis:
                            w.write(str(x)+' '+str(y)+' '+str(z)+' '+str(intensity)+' '+str(ring)+'\n')
                        else:
                            current += 1

            with open(fileout, 'r') as w:
                data = w.readlines()
            w, numw = data[6].split(' ', 1)
            p, nump = data[9].split(' ', 1)
            nump = numw = int(numw) - current -1
            data[6] = 'WIDTH ' + str(numw) + '\n'
            data[9] = 'POINTS ' + str(nump) + '\n'
            with open(fileout, 'w') as w:
                w.writelines(data)
        print('ok')
        rospy.Rate(10).sleep()
        
if __name__ == '__main__':
    map = map(0, 0, 0, 3, '/home/eric/Desktop/')
    rospy.init_node('map_cut')
    while(not rospy.is_shutdown()):
        map.cut('100611000.pcd')
