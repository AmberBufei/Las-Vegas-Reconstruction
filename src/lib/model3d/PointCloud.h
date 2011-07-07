/*
 * PointCloud.h
 *
 *  Created on: 02.09.2008
 *      Author: twiemann
 */

#ifndef POINTCLOUD_H_
#define POINTCLOUD_H_

#include "Renderable.h"
#include "ColorVertex.h"

#include <vector>
#include <string>
#include <fstream>

using namespace std;

class PointCloud : public Renderable{
public:
    PointCloud(string filename);
    PointCloud();

    virtual ~PointCloud();
    virtual inline void render();

    vector<ColorVertex> getPoints(){return points;};
    void setPoints(){};

    void addPoint(float x, float y, float z, uchar r, uchar g, uchar b){
        m_boundingBox->expand(ColorVertex(x, y, z, r, g, b));
        points.push_back(ColorVertex(x, y, z, r, g, b));
    };

    void addPoint(const ColorVertex v) {
        m_boundingBox->expand(v);
        points.push_back(v);
    };

    void clear(){
        delete m_boundingBox;
        m_boundingBox = new BoundingBox;
        points.clear();
    };

    void updateDisplayLists();
//private:
    vector<ColorVertex> points;


private:
    int getFieldsPerLine(string filename);

};

inline void PointCloud::render()
{
    if(listIndex != -1)
    {
        glPushMatrix();
        glMultMatrixf(transformation.getData());

        if(active)
        {
            glCallList(activeListIndex);
        }
        else
        {
            glCallList(listIndex);
        }
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }
}

#endif /* POINTCLOUD_H_ */
