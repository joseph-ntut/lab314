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
        
        current = 0
        file = self.diction + filein

        with open(file,'r') as f :
            max_x = min_x = max_y = min_y = 0.0
            line = f.readlines()
            for i in range(11,len(line)-1):
                a, b, c, d, e = line[i].split(' ',4)
                if float(a) >= max_x:
                    max_x = float(a)
                if float(b) >= max_y:
                    max_y = float(b)
                if float(a) <= min_x:
                    min_x = float(a)
                if float(b) <= min_y:
                    min_y = float(b)
            grid_x = int(( max_x - min_x ) / ( 2 * self.dis ))
            grid_y = int(( max_y - min_y ) / ( 2 * self.dis ))

        file_array = []

        if grid_x % 2 == 0:
            grid_x += 1
        if grid_y % 2 == 0:
            grid_y += 1

        for x in range(grid_x):
            gx = x - ((grid_x - 1) / 2)
            for y in range(grid_y):
                gy = y - ((grid_y - 1) / 2)
                file_array.append(self.diction + str(gx) + 'x' + str(gy) + '.pcd')
                fileout = self.diction + str(gx) + 'x' + str(gy) + '.pcd'
                dx = gx * self.dis
                dy = gy * self.dis

                with open(file, 'r') as f:
                    with open(fileout, 'w') as w:
                        line = f.readlines()
                        for i in range(11):
                            w.write(line[i])
                        for i in range(11,len(line)-1):
                            a, b, c, d, e = line[i].split(' ',4)
                            x = float(a)
                            y = float(b)
                            z = float(c)
                            intensity = int(d)
                            ring = int(e)

                            if x + dx > self.px - self.dis and x + dx < self.px + self.dis and y + dy > self.py - self.dis and y + dy < self.py + self.dis and z > self.pz - self.dis and z < self.pz + self.dis:
                                w.write(str(x)+' '+str(y)+' '+str(z + 1.25)+' '+str(intensity)+' '+str(ring)+'\n')
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
