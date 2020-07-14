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

        with open(file,'r') as f :
            max_x = min_x = max_y = min_y = 0.0
            line = f.readlines()
            for i in range(11,len(line)-1):
                a, b, c, d = line[i].split(' ',3)
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

        for cx in range(grid_x):
            gx = cx - ((grid_x - 1) / 2)
            for cy in range(grid_y):
                gy = cy - ((grid_y - 1) / 2)
                file_array.append(self.diction + str(gx) + 'x' + str(gy) + '.pcd')
                fileout = self.diction + str(gx) + 'x' + str(gy) + '.pcd'
                dx = gx * self.dis
                dy = gy * self.dis

                current = 0
                with open(file, 'r') as f:
                    with open(fileout, 'w') as w:
                        line = f.readlines()
                        for i in range(11):
                            w.write(line[i])
                        for i in range(11,len(line)-1):
                            a, b, c, d = line[i].split(' ',3)
                            x = float(a)
                            y = float(b)
                            z = float(c)
                            intensity = int(d)
                            "ring = int(e)"

                            if x + 2 * dx > self.px - self.dis and x + 2 * dx < self.px + self.dis and y + 2 * dy > self.py - self.dis and y + 2 * dy < self.py + self.dis:
                                w.write(str(x)+' '+str(y)+' '+str(z)+' '+str(intensity)+'\n')
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
    rospy.init_node('map_cut')
    while(not rospy.is_shutdown()):
        init_x = rospy.get_param('x',default = 0)
        init_y = rospy.get_param('y',default = 0)
        init_z = rospy.get_param('z',default = 0)
        init_d = rospy.get_param('d',default = 7)
        init_dir = rospy.get_param('dir',default = '/home/eric/Desktop/map2/2/')
        map_file = rospy.get_param('filename',default = '0.pcd')
        map = map(init_x, init_y, init_z, init_d, init_dir)
        map.cut(map_file)
