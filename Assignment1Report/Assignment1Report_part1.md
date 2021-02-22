The algorithim implimentation
=============================

In words:
---------

I handled each case alone: first the two cases which Delta X or Delta Y
is 0 which means the line either vertical or horizontal, then i drew a
regural line by looping over the x or the y coordinates. after that I
checked each case of a (defined in the Rasterization lecture slides) but
there is a twist like when 0 \< a \< 1 then there are two cases either
Delta X and Delta Y are both negative or both positive but in case they
were both positive all i had to do is to make a recursive call with
swapped points like: `DrawLine(p2,p1,color)` and the original call was
`DrawLine(p1,p2,color)`. there are 3 cases like this handled by a
recursive call and the rest are by the lecture slides.

In code:
--------

    void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
    {
        // TODO: Implement bresenham algorithm
        int x = p1.x, y = p1.y, ReflectFlag = 0, LoopVar = p2.x;
        double deltaP = (p2.x - p1.x), deltaQ = (p2.y - p1.y);
        double a = deltaQ / deltaP, e = (-1) * deltaP;
        if (deltaQ == 0)
        {
            if(deltaP>0)
            while (x <= p2.x)
            {
                PutPixel(x, y, color);
                x = x + 1;
            }
            else
            {
                while (x >= p2.x)
                {
                    PutPixel(x, y, color);
                    x = x - 1;
                }
            }
        }else
            if (deltaP == 0)
            {
                if (deltaQ > 0)
                    while (y <= p2.y)
                    {
                        PutPixel(x, y, color);
                        y = y + 1;
                    }
                else
                    while (y >= p2.y)
                    {
                        PutPixel(x, y, color);
                        y = y - 1;
                    }
            }
            else
        if (a > 1 && !(a < -1)) //switch x and y
        {
            x = p1.y;
            y = p1.x;
            deltaQ = deltaP;
            deltaP = (p2.y - p1.y);
            if (deltaQ > 0 && deltaP > 0) {
                while (x <= p2.y)
                {
                    if (e > 0)
                    {
                        y = y + 1;
                        e = e - (2 * deltaP);
                    }
                    PutPixel(y, x, color);
                    x = x + 1;
                    e = e + (2 * deltaQ);
                }
            }
            else
                DrawLine(p2, p1, color);
        }
        else
        if ((a > -1 && a < 0) && !(a < -1)) //reflect 
        {
            if (deltaQ < 0) {
                deltaQ = (-1) * deltaQ;
                while (x <= p2.x)
                {
                    if (e > 0)
                    {
                        y = y - 1;
                        e = e - (2 * deltaP);
                    }
                    PutPixel(x, y, color);
                    x = x + 1;
                    e = e + (2 * deltaQ);
                }
            }
            else
            {
                deltaP = (-1) * deltaP;
                    while (x >= p2.x)
                    {
                        if (e > 0)
                        {
                            if (y < p2.y)
                                y = y + 1;
                            e = e - (2 * deltaP);
                        }
                        PutPixel(x, y, color);
                        x = x - 1;
                        e = e + (2 * deltaQ);
                    }
            }
        }
        else
        if (a < -1)//swtich and reflect
        {
            x = p1.y;
            y = p1.x;
            deltaQ = deltaP;
            deltaP = (p2.y - p1.y);
            LoopVar = p2.y;
            if (deltaQ < 0) {
                deltaP = (-1) * deltaP;
                while (x <= LoopVar)
                {
                    if (e < 0)
                    {
                        y = y - 1;
                        e = e - (2 * deltaP);
                    }
                    PutPixel(y, x, color);
                    x = x + 1;
                    e = e + (2 * deltaQ);
                }
            }
            else
                DrawLine(p2, p1, color);
        }
        else
        {
            if (deltaP > 0 && deltaQ > 0) {
                while (x <= p2.x)
                {
                    if (e > 0)
                    {
                        y = y + 1;
                        e = e - (2 * deltaP);
                    }
                    PutPixel(x, y, color);
                    x = x + 1;
                    e = e + (2 * deltaQ);
                }
            }
            else
                DrawLine(p2, p1, color);
        }
    }

Circle image
------------

![Cricle Image](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/Circle.JPG)

My be creative image my favorite TV series :)
------------

![FRIENDS Image](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/FRIENDS.JPG)
