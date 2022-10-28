import cv2 as cv
import numpy as np

FOCAL_LENGTH = 1

class Triangle:
    def __init__(self, a, b, c):
        self.vertex = [a, b, c]
        self.a = a
        self.b = b
        self.c = c

    def barycentric(self, p):
        # solve x*a+y*b+z*c = p

        # vs
        v0 = [self.b[0] - self.a[0], self.b[1] - self.a[1]]
        v1 = [self.c[0] - self.a[0], self.c[1] - self.a[1]]
        v2 = [self.p[0] - self.a[0], self.p[1] - self.a[1]]

        den = v0[0]*v1[1] - v0[1]*v1[0] 

        x = (v2[0]*v1[1] - v2[1]*v1[0]) / den
        y = (v0[0]*v2[1] - v0[1]*v2[0]) / den
        z = 1 - x -y

        return [x, y, z]

def project_vertex(vertex, focal_length):
    # preject a vertex on the plan z
    x, y, z = vertex

    x_projected = (focal_length * x) / (z+focal_length)
    y_projected = (focal_length * y) / (z+focal_length)

    return [x_projected, y_projected]

def draw_line2d(start, stop):
    points = [[int(start[0]), int(start[1])]]

    dx = stop[0] - start[0]
    dy = stop[1] - start[1]


    x, y = start[0], start[1]

    if dx == 0:
        d = dy/abs(dy)

        while (int(y) != int(stop[1])):
            y += d
            points.append([int(start[0]), int(y)])


    elif dy == 0:
        d = dx/abs(dx)

        while (int(x) != int(stop[0])):
            x += d
            points.append([int(x), int(start[1])])

    elif abs(dx) < abs(dy):
        d = dx/dy
        while (x != stop[0] and y != stop[1]):
            y += 1
            x += d
            points.append([int(x), int(y)])
    else:
        d = dy/dx
        while (x != stop[0] and y != stop[1]):
            y += d
            x += 1
            points.append([int(x), int(y)])

    return points

def render(vertex, edges, shape):
    
    # preject
    v_projected_pos = []
    
    for v in vertex:
        v_projected_pos.append(project_vertex(v, FOCAL_LENGTH))

    # draw frame
    frame = np.zeros(shape)
    
    for e in edges:
        for point in draw_line2d(v_projected_pos[e[0]], v_projected_pos[e[1]]):
            
            # check pos
            if (0 <= point[0] < shape[0] and 0 <= point[1] < shape[1]):
                frame[point[0], point[1]] = 1

    return frame

# draw a square


vertex = [[100, 1,   1],
          [100, 100, 1],
          [1,   100, 1],
          [1,   1,   1]]

edges = [[0, 1],
         [0, 3],
         [2, 1],
         [2, 3]]

cv.imshow('name', render(vertex, edges, (128, 128)))
cv.waitKey(0)