#ifndef COLLIDER_H
#define COLLIDER_H
#include <math.h>
#include <float.h>
#include <array>
#include <limits>
#include <algorithm>

#include "Vector.h"
#include "Mesh.h"

#define MAX_EPA_ITERATIONS 16

struct Simplex
{
    std::array<Vector<float>, 4> points;
    int size = 0;
    //Pushes new vector onto the back of the array
    void set(std::vector<Vector<float>> list)
    {
        size = 0;
        for (int i = 0; i < list.size(); i++)
        {
            points[i] = list[i];
        }
    }
    void pushPoint(Vector<float> p)
    {
        if (size > 4)
        {
            printf("ERROR Too large simplex!\n");
            return;
        }
        else
        {
            points[size] = p;
            size++;
        }
    }
    //Pops element of given index, shifting other elements
    void
    pop(int index)
    {
        if (index > size - 1)
            return;
        for (int i = index; i < size - 1; i++)
        {
            points[i] = points[i + 1];
        }
        size--;
    }
    //Pops the front element out of the array, shifting other elements forward
    void popFront()
    {
        if (!size)
            return;
        for (int i = 0; i < size - 1; i++)
        {
            points[i] = points[i + 1];
        }
        size--;
    }
};

struct Polytope
{
    void set(Simplex &s)
    {
        for (int i = 0; i < s.points.size(); i++)
        {
            points.push_back(s.points[i]);
        }
    }
    std::vector<Vector<float>> points;
    void pushPoint(Vector<float> p)
    {
        points.push_back(p);
    }
    void pushPoint(Vector<float> p, int i)
    {
        points.insert(points.begin() + i, p);
    }
};

class Collider
{
public:
    Collider()
    {
    }
    Collider(std::vector<Vector<float>> t_vertices)
    {
        vertices = t_vertices;
    }
    //Support function for meshes.
    //Find the most distant vertex in given direction
    Vector<float> FurthestPoint(Vector<float> dir)
    {
        float maxDotProduct = -FLT_MAX;
        Vector<float> supportPoint;

        for (int i = 0; i < vertices.size(); i++)
        {
            //Find dot product of each vertex and given direction
            float dotProduct = vertices[i].dotProduct(dir);
            //Find the maximum dot product
            if (dotProduct > maxDotProduct)
            {
                maxDotProduct = dotProduct;
                supportPoint = vertices[i];
            }
        }
        //Return the point that has highest dot product
        return supportPoint;
    }

    Vector<float> FurthestPoint(Vector<float> dir, std::vector<Vector<float>> t_vertices, Vector<float> t_offset)
    {
        float maxDotProduct = -FLT_MAX;
        Vector<float> supportPoint;

        for (int i = 0; i < t_vertices.size(); i++)
        {
            //Find dot product of each vertex and given direction
            float dotProduct = t_vertices[i].dotProduct(dir);
            //Find the maximum dot product
            if (dotProduct > maxDotProduct)
            {
                maxDotProduct = dotProduct;
                supportPoint = t_vertices[i];
            }
        }
        //Return the point that has highest dot product
        supportPoint.x += t_offset.x;
        supportPoint.y += t_offset.y;
        supportPoint.z += t_offset.z;
        return supportPoint;
    }

    Vector<float> Support(Mesh *m1, Mesh *m2, Vector<float> dir)
    {
        Vector<float> negativeDir;
        negativeDir.x = dir.x * -1;
        negativeDir.y = dir.y * -1;
        negativeDir.z = dir.z * -1;
        Vector<float> supportPoint1 = FurthestPoint(dir, m1->GetRotatedVertices(), m1->m_pos);
        Vector<float> supportPoint2 = FurthestPoint(negativeDir, m2->GetRotatedVertices(), m2->m_pos);
        return Vector<float>(supportPoint1.x - supportPoint2.x, supportPoint1.y - supportPoint2.y, supportPoint1.z - supportPoint2.z);
    }

    bool GJK(Mesh *m1, Mesh *m2)
    {
        Vector<float> dir = (m2->m_pos - m1->m_pos).normalize();
        Vector<float> initialPoint = Support(m1, m2, dir);

        Simplex s;
        s.pushPoint(initialPoint);
        dir = Vector<float>(0, 0, 0) - s.points[0];
        int i = 0;
        while (1)
        {
            dir.normalize();
            Vector<float> A = Support(m1, m2, dir);

            if (A.dotProduct(dir) < 0)
            {
                return false;
            }
            s.pushPoint(A);
            if (CheckSimplex(&s, &dir))
            {
                return true;
            }
        }
    }

private:
    bool CheckSimplex(Simplex *s, Vector<float> *dir)
    {
        if (s->size == 2)
        {
            return CheckLine(s, dir);
        }
        else if (s->size == 3)
        {
            return CheckTriangle(s, dir);
        }
        else if (s->size == 4)
        {
            return CheckTetra(s, dir);
        }
    }

    bool CheckLine(Simplex *s, Vector<float> *dir)
    {
        Vector<float> A = s->points[1];
        Vector<float> B = s->points[0];
        Vector<float> AB = B - A;
        Vector<float> AO = Vector<float>(0, 0, 0) - A;
        AB.normalize();
        AO.normalize();
        //Triple product, yielding to us vector perpendicular to vector AB pointing towards AO
        Vector<float> ABAO = AB.crossProduct(AO).normalize();
        Vector<float> ABperp = ABAO.crossProduct(AB).normalize();
        if (AB.dotProduct(AO) > 0)
        {
            dir->set(ABperp);
        }
        else
        {
            s->points = {A};
            dir->set(AO);
        }
        //Does not check for cases where origin lies on the line yet.
        return false;
    }

    bool CheckTriangle(Simplex *s, Vector<float> *dir)
    {
        Vector<float> A = s->points[2];
        Vector<float> B = s->points[1];
        Vector<float> C = s->points[0];

        Vector<float> AB = B - A;
        Vector<float> AC = C - A;
        Vector<float> AO = Vector<float>(0, 0, 0) - A;
        AO.normalize();

        //Vector pointing from line AB to region rAB
        Vector<float> ACAB = AC.normalize().crossProduct(AB.normalize());
        Vector<float> ABperp = ACAB.crossProduct(AB.normalize()).normalize();

        //Vector pointing from line AC to region rAC
        Vector<float> ABAC = AB.crossProduct(AC.normalize()).normalize();
        Vector<float> ACperp = ABAC.crossProduct(AC.normalize()).normalize();

        //Check if vector ABperp points towards the origin
        if (ABperp.dotProduct(AO) > 0.f)
        {
            //Remove oldest vector (C)
            s->pop(0);
            //Set direction for looking for new vectors as ABperp
            dir->set(ABperp);
            return false;
        }
        //Check if vector ACperp points towards the origin
        if (ACperp.dotProduct(AO) > 0.f)
        {
            //Remove vector B
            s->pop(1);
            //Set direction for looking for new vectors as ACperp
            dir->set(ACperp);
            return false;
        }
        if (ABAC.dotProduct(AO) > 0.f)
        {
            dir->set(ABAC);
        }
        else
        {
            dir->set(ABAC * -1.0f);
        }
        //Origin does not occur in regions rAB or rAC, thus it must be in the triangle
        return false;
    }

    bool CheckTetra(Simplex *s, Vector<float> *dir)
    {
        Vector<float> A = s->points[3];
        Vector<float> B = s->points[2];
        Vector<float> C = s->points[1];
        Vector<float> D = s->points[0];

        Vector<float> AB = B - A;
        Vector<float> AC = C - A;
        Vector<float> AD = D - A;

        //Normals of each tetrahedron's side triangles.
        Vector<float> ABAC = AB.normalize().crossProduct(AC.normalize());
        Vector<float> ACperp = ABAC.crossProduct(AC.normalize()).normalize();

        Vector<float> ADAB = AD.normalize().crossProduct(AB.normalize());
        Vector<float> ABperp = ADAB.crossProduct(AB.normalize()).normalize();

        Vector<float> ACAD = AC.normalize().crossProduct(AD.normalize());
        Vector<float> ADperp = ACAD.crossProduct(AD.normalize()).normalize();

        Vector<float> AO = Vector<float>(0, 0, 0) - A;
        //AO.normalize();

        if (ACperp.dotProduct(AO) > 0.f)
        {
            Simplex t1;
            t1.points = {A, B, C};
            return CheckTriangle(&t1, dir);
        }
        if (ABperp.dotProduct(AO) > 0.f)
        {
            Simplex t2;
            t2.points = {A, D, B};
            return CheckTriangle(&t2, dir);
        }
        if (ADperp.dotProduct(AO) > 0.f)
        {
            Simplex t3;
            t3.points = {A, C, D};
            return CheckTriangle(&t3, dir);
        }

        return true;
    }

    std::vector<Vector<float>> vertices;
};
#endif