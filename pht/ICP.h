/*
 * ICP.h
 *
 *  Created on: Jan 31, 2017
 *      Author: linh
 */

#ifndef ICP_H_
#define ICP_H_
void icpMethod(Image modelGray, Image &sceneGray);
void icpMethod2(Image modelGray, Image &sceneGray);
void icpFolder(string folderScene, vector<string> sceneImages, Image model);
#endif /* ICP_H_ */
