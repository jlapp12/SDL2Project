#include "screen.h"
#include <SDL2/SDL.h>
#include <vector>
#include <numeric>
#include <cmath> //necessary for length pythagorean theorem std::sqrt

struct vec3
{
        float x,y,z; //2D picture -- data must be tracked for z-axis
};


struct connection
{
        int a, b;
};


void rotate(vec3& point, float x = 1, float y = 1, float z = 1)
{
        /*
        This function is used to rotate the points. This is a 2D picture. 
        Each point must be put into relation compared to the other via the z-axis.
        Rotate function references vec3, passes three arguments. Amount wanted to rotate along each axis.

        The rotation is then applied to each.
        Every value is passed in, (x, y, z), assign it to rad (radian).
        Do the rotation to every assigned value. 

        Points must be defined now.
        */


       float rad = 0;
       rad = x;
       point.y = std::cos(rad) * point.y - std::sin(rad) * point.z;
       point.z = std::sin(rad) * point.y + std::cos(rad) * point.z;

       rad = y;
       point.x = std::cos(rad) * point.x + std::sin(rad) * point.z;
       point.z = -std::sin(rad) * point.x + std::cos(rad) * point.z;

       rad = z;
       point.x = std::cos(rad) * point.x - std::sin(rad) * point.y;
       point.y = std::sin(rad) * point.x + std::cos(rad) * point.y;



}





void line(Screen&  screen, float x1, float y1, float x2, float y2)
{
        float dx = x2 - x1;
        float dy = y2 - y1;

         float length = std::sqrt(dx * dx + dy * dy);  
        float angle = std::atan2(dy, dx); //angle of line. i.e. 45 degree angle will be given if line is on 45 degree

         for(float i = 0; i < length; i++)
         {
               screen.pixel(
                                x1 + std::cos(angle) * i,
                                y1 + std::sin(angle) * i //gives forward vector and scales it by i
               );
         }
}


int main(int argc, char* argv[])
{
        Screen screen;

        std::vector<vec3> points {
                /*
                This function goes through every point and renders it out.
                It needs a centroid. (centroid of cube)

                
                */
                {100, 100, 100},
                {200, 100, 100},
                {200, 200, 100},
                {100, 200, 100},
                
                {100, 100, 200},
                {200, 100, 200},
                {200, 200, 200},
                {100, 200, 200}

        };


        std::vector<connection> connections
        /*
        need to stich together lines to make it look like a cube. (draw line function)
        this can be done by creating a struct, connections which will hold the cube sections
        **struct defined above**

        in vec3, there are 8 points. we need more than 8 points.
        more than 8 points is necessary due to the way a cube is I believe.
        the sections connecting vertices total 12, not 8.

         define the vertices of the point that we want connected

        line {0,4} connects the first point with the fifth point
        imagine a cube. 8 vertices
        point1 needs to be connected to point 4.
        the wall needs to be connected to other wall to create 3D shape.


        */
        {
                {0,4},
                {1,5},
                {2,6},
                {3,7},

                {0,1},
                {1,2},
                {2,3},
                {3,0},

                {4,5},
                {5,6},
                {6,7},
                {7,4}

        };

        //Calculate centroid

        vec3 centroid{0, 0, 0};
        /*
        
        new vec3 -- the centroid of cube.
        go through every point of points.
        assign to the centroid the summation of points. (add total to centroid)
        calculates centre point of object.
        
        divide centroid by size of vector.

        */
        for(auto& p : points)
        {
                centroid.x += p.x;
                centroid.y += p.y;
                centroid.z += p.z;

        }

        centroid.x /= points.size();
        centroid.y /= points.size();
        centroid.z /= points.size();




        while(true)
        {
                /*
                rotate all points, and then display
                
                for loop is necessary. takes points.
                first rotate, then display. (rotate function from above)
                pass point, then rotation amount
                show pixel.. then clear
                cannot keep drawing pixels. accumulating too many pixels will lead to ram issues




                centroid is created.
                subtract the centroid by the point we are rotated about. once rotated add it back. 

                */
                for(auto& p : points)
                {
                        p.x -= centroid.x;
                        p.y -= centroid.y;
                        p.z -= centroid.z;
                        rotate(p, 0.002, 0.001, 0.004);
                        p.x += centroid.x;
                        p.y += centroid.y;
                        p.z += centroid.z;

                        screen.pixel(p.x, p.y);


                }
                for (auto& conn : connections)
                {
                        /*
                        
                        This for loop connects all of the vertices.
                        Connects the vertices using line function.
                        
                        */
                        line(screen, points[conn.a].x,
                                        points[conn.a].y,
                                        points[conn.b].x,
                                        points[conn.b].y);
                }
                screen.show();
                screen.clear();
                screen.input(); //if user click x, exit program
        }       SDL_Delay(30);
        return 0;
}