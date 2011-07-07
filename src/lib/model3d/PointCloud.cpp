/*
 * PointCloud.cpp
 *
 *  Created on: 02.09.2008
 *      Author: twiemann
 */

#include "PointCloud.h"

#include <string.h>

PointCloud::PointCloud()
{
}

PointCloud::PointCloud(string filename) : Renderable(filename){

    ifstream in(filename.c_str());

    m_boundingBox = new BoundingBox;

    if(!in.good()){
        cout << "##### Error: Could not open file " << filename << "." << endl;
        return;
    }

//    int i = 0;
//    float x, y, z, dummy;
//    while(in.good()){
//        if(i > 0 && i % 100000 == 0) cout << "##### READING POINTS: " << i << endl;
//        in >> x >> y >> z >> dummy;
//
//        m_boundingBox->expand(Vertex(x,y,z));
//        points.push_back(ColorVertex(x,y,z));
//        i++;
//    }


    //Get number of data fields to ignore
    int number_of_dummys = getFieldsPerLine(filename) - 3;
    int c = 0;
	 int read_color = ( number_of_dummys == 4 );
	 
	 if ( read_color ) {
		 number_of_dummys -= 3;
	 }

    //Point coordinates
    float x, y, z, dummy;
	unsigned int r, g, b;

    //if(in.good()) in >> dummy;

    //Read file
    while(in.good() ){
    	in >> x >> y >> z;

		/* Ignore reflection, … */
    	for(int i = 0; i < number_of_dummys; i++){
    		in >> dummy;
    	}
		
		/* Read colors */
		if ( read_color ) {
			in >> r >> g >> b;
			m_boundingBox->expand( ColorVertex( x, y, z, (uchar) r, (uchar) g, (uchar) b ) );
			points.push_back( ColorVertex( x, y, z, (uchar) r, (uchar) g, (uchar) b ) );
		} else {
			m_boundingBox->expand(x, y, z);
//    	if(c % 1 == 0) // <- This is true for all c \in Z 
			points.push_back(ColorVertex(x,y,z));
		}
    	c++;

    	if ( c % 100000 == 0 ) {
			cout << "##### Reading Points... " << c << endl;
		}
    }

    cout << "Loaded Points: " << points.size() << endl;

    updateDisplayLists();
}

void PointCloud::updateDisplayLists(){

    // Check for existing display list for normal rendering
    if(listIndex != -1) {
        cout<<"PointCloud::initDisplayList() delete display list"<<endl;
        glDeleteLists(listIndex,1);
    }

    // Create new display list and render points
    listIndex = glGenLists(1);
    glNewList(listIndex, GL_COMPILE);
    glBegin(GL_POINTS);

    for(size_t i = 0; i < points.size(); i++)
    {
        glColor3f( ( (float) points[i].r ) / 255,
                   ( (float) points[i].g ) / 255,
                   ( (float) points[i].b ) / 255 );

        glVertex3f(points[i].x,
                   points[i].y,
                   points[i].z);
    }
    glEnd();
    glEndList();

    // Check for existing list index for rendering a selected point
    // cloud

    if(activeListIndex != -1)
    {
        cout<<"PointCloud::initDisplayList() delete  active display list"<<endl;
        glDeleteLists(activeListIndex,1);
    }

    activeListIndex = glGenLists(1);
    glNewList(activeListIndex, GL_COMPILE);
    glBegin(GL_POINTS);

    glColor3f(1.0, 1.0, 0.0);
    for(size_t i = 0; i < points.size(); i++)
    {

        glVertex3f(points[i].x,
                   points[i].y,
                   points[i].z);
    }
    glEnd();
    glEndList();




}

int PointCloud::getFieldsPerLine(string filename)
{
	ifstream in(filename.c_str());

	//Get first line from file
	char first_line[1024];
	in.getline(first_line, 1024);
	in.close();

	//Get number of blanks
	int c = 0;
	char* pch = strtok(first_line, " ");
	while(pch != NULL){
		c++;
		pch = strtok(NULL, " ");
	}

	in.close();

	return c;
}

PointCloud::~PointCloud() {
    // TODO Auto-generated destructor stub
}
