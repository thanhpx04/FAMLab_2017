/*
 * Thresholds.cpp
 *
 *  Created on: Nov 26, 2016
 *      Author: linh
 */
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <time.h>
#include <float.h>

using namespace std;
#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Edge.h"
#include "../imageModel/Matrix.h"

#include "Filters.h"

#include "Thresholds.h"
const int BIN_SIZE = 256;

ptr_IntMatrix binaryThreshold(ptr_IntMatrix inputMatrix, int tValue,
                              int maxValue)
{
    int rows = inputMatrix->getRows();
    int cols = inputMatrix->getCols();

    /*Matrix<double> kernel = getGaussianKernel(9,1);
     *inputMatrix = gaussianBlur(*inputMatrix,kernel);*/

    ptr_IntMatrix result = new Matrix<int>(rows, cols, maxValue);
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (inputMatrix->getAtPosition(r, c) > tValue)
                result->setAtPosition(r, c, maxValue);
            else
                result->setAtPosition(r, c, 0);
        }
    }

    return result;
}
/*

 ptr_IntMatrix postProcess(ptr_IntMatrix binaryMatrix, int maxValue)
 {
 int rows = binaryMatrix->getRows();
 int cols = binaryMatrix->getCols();
 ptr_IntMatrix result = new Matrix<int>(rows, cols, maxValue);
 *result = *binaryMatrix;
 Point left(0, 0), right(0, 0);
 vector<Line> lines;

 for (int r = 1; r < rows - 1; r++)
 {
 for (int c = 1; c < cols - 1; c++)
 {
 left.setX(0);
 left.setY(0);
 right.setX(0);
 right.setY(0);
 lines.clear();
 int value = binaryMatrix->getAtPosition(r, c);
 int valuel = binaryMatrix->getAtPosition(r, c - 1);
 if (value == maxValue && valuel == 0)
 {

 // xac dinh diem dau tien
 left.setX(c - 1);
 left.setY(r);
 if (left.getX() != 0)
 {
 // xac dinh diem thu 2
 for (int k = c; k < cols; k++)
 {
 if (binaryMatrix->getAtPosition(r, k) == 0
 && binaryMatrix->getAtPosition(r, k - 1) == maxValue)
 {
 right.setX(k);
 right.setY(r);
 goto checkbghole;
 }
 }
 }
 // kiem tra hole

 checkbghole: if (left != 0 && right != 0 && right.getX() > left.getX())
 {
 // do dong truoc do
 bool beginHole = true;
 if (r - 1 < 0 || r - 1 >= rows)
 beginHole = false;
 else
 {
 for (int l = left.getX() + 1; l <= right.getX() - 1; l++)
 {
 if (binaryMatrix->getAtPosition(r - 1, l) == maxValue)
 {
 beginHole = false;
 goto resetValues;
 }
 }
 }
 resetValues: if (beginHole == false)
 {
 left.setX(0);
 left.setY(0);
 right.setX(0);
 right.setY(0);
 }
 }
 if (left != 0 && right != 0)
 {
 bool inhole = true;
 int rnew = r + 1;
 int clnew = 0, crnew = 0;
 lines.push_back(Line(left, right));
 do
 {
 clnew = 0;
 crnew = 0;
 if (rnew < rows)
 {
 // check dong do
 inhole = false;
 for (int n = left.getX() + 1; n <= right.getX() - 1; n++)
 {
 if (n < cols && rnew < rows
 && binaryMatrix->getAtPosition(rnew, n) == maxValue)
 {
 inhole = true;
 goto checkinhole;
 }
 }
 checkinhole: if (inhole)
 {
 if (binaryMatrix->getAtPosition(rnew, left.getX()) == maxValue)
 {
 for (int l = left.getX(); l > 0; l--)
 {
 if (binaryMatrix->getAtPosition(rnew, l) == 0)
 {
 clnew = l;
 goto checkright;
 }
 }
 }
 checkright: if (binaryMatrix->getAtPosition(rnew, right.getX())
 == maxValue)
 {
 for (int m = right.getX(); m >= left.getX(); m--)
 {
 if (binaryMatrix->getAtPosition(rnew, m) == 0)
 {
 crnew = m;
 goto checkandpush;
 }
 }

 }
 checkandpush: if (clnew != 0 && crnew != 0)
 {
 left.setX(clnew);
 left.setY(rnew);
 right.setX(crnew);
 right.setY(rnew);
 lines.push_back(Line(left, right));
 }
 }
 }
 else
 {
 lines.clear();
 inhole = false;
 }
 rnew++;

 }
 while (inhole);

 for (size_t li = 0; li < lines.size(); li++)
 {
 Line line = lines.at(li);
 if (line.getBegin().getY() == line.getEnd().getY())
 {
 for (int x = line.getBegin().getX(); x < line.getEnd().getX();
 x++)
 {
 binaryMatrix->setAtPosition(line.getBegin().getY(), x, 0);
 }
 }
 }
 lines.clear();
 }

 }
 }
 }

 return binaryMatrix;
 }
 */
ptr_IntMatrix postProcess(ptr_IntMatrix binaryMatrix, int maxValue)
{
    int rows = binaryMatrix->getRows();
    int cols = binaryMatrix->getCols();
    ptr_IntMatrix result = new Matrix<int>(rows, cols, maxValue);
    *result = *binaryMatrix;
    Point left(0, 0), right(0, 0);
    vector<Line> lines;

    for (int r = 1; r < rows - 1; r++)
    {
        for (int c = 1; c < cols - 1; c++)
        {
            left.setX(0);
            left.setY(0);
            right.setX(0);
            right.setY(0);
            lines.clear();
            int value = binaryMatrix->getAtPosition(r, c);
            int valuel = binaryMatrix->getAtPosition(r, c - 1);
            if (value == maxValue && valuel == 0)
            {

                // xac dinh diem dau tien
                left.setX(c - 1);
                left.setY(r);
                if (left.getX() != 0)
                {
                    // xac dinh diem thu 2
                    for (int k = c; k < cols; k++)
                    {
                        if (binaryMatrix->getAtPosition(r, k) == 0
                                && binaryMatrix->getAtPosition(r, k - 1) == maxValue)
                        {
                            right.setX(k);
                            right.setY(r);
                            goto checkbghole;
                        }
                    }
                }
                // kiem tra hole

checkbghole: if (left != 0 && right != 0 && right.getX() > left.getX())
                {
                    // do dong truoc do
                    bool beginHole = true;
                    if (r - 1 < 0 || r - 1 >= rows)
                        beginHole = false;
                    else
                    {
                        for (int l = left.getX() + 1; l <= right.getX() - 1; l++)
                        {
                            if (binaryMatrix->getAtPosition(r - 1, l) == maxValue)
                            {
                                beginHole = false;
                                goto resetValues;
                            }
                        }
                    }
resetValues: if (beginHole == false)
                    {
                        left.setX(0);
                        left.setY(0);
                        right.setX(0);
                        right.setY(0);
                    }
                }
                if (left != 0 && right != 0)
                {
                    bool inhole = true;
                    int rnew = r + 1;
                    int clnew = 0, crnew = 0;
                    lines.push_back(Line(left, right));
                    do
                    {
                        clnew = 0;
                        crnew = 0;
                        if (rnew < rows)
                        {
                            // check dong do
                            inhole = false;
                            for (int n = left.getX() + 1; n <= right.getX() - 1; n++)
                            {
                                if (n < cols && rnew < rows
                                        && binaryMatrix->getAtPosition(rnew, n) == maxValue)
                                {
                                    inhole = true;
                                    goto checkinhole;
                                }
                            }
checkinhole: if (inhole)
                            {
                                // check left;
                                int mindist = DBL_MAX;
                                for (int l = left.getX() - 10; l <= left.getX() + 10; l++)
                                {
                                    if (binaryMatrix->getAtPosition(rnew, l) == 0)
                                    {
                                        if (abs(left.getX() - l) < mindist)
                                        {
                                            clnew = l;
                                            mindist = abs(left.getX() - l);
                                        }
                                    }
                                }

                                mindist = DBL_MAX;
checkright: for (int m = right.getX() - 10;
                                m <= right.getX() + 10; m++)
                                {
                                    if (binaryMatrix->getAtPosition(rnew, m) == 0)
                                    {
                                        if (abs(right.getX() - m) < mindist)
                                        {
                                            crnew = m;
                                            mindist = abs(right.getX() - m);
                                            //goto checkandpush;
                                        }
                                    }
                                }
                                /*if (binaryMatrix->getAtPosition(rnew, right.getX())
                                 == maxValue)
                                 {
                                 for (int m = right.getX(); m >= left.getX(); m--)
                                 {
                                 if (binaryMatrix->getAtPosition(rnew, m) == 0)
                                 {
                                 crnew = m;
                                 goto checkandpush;
                                 }
                                 }

                                 }*/
checkandpush: if (clnew != 0 && crnew != 0)
                                {
                                    left.setX(clnew);
                                    left.setY(rnew);
                                    right.setX(crnew);
                                    right.setY(rnew);
                                    lines.push_back(Line(left, right));
                                }

                            }
                        }
                        else
                        {
                            lines.clear();
                            inhole = false;
                        }
                        rnew++;

                    }
                    while (inhole);

                    for (size_t li = 0; li < lines.size(); li++)
                    {
                        Line line = lines.at(li);
                        if (line.getBegin().getY() == line.getEnd().getY())
                        {
                            for (int x = line.getBegin().getX(); x < line.getEnd().getX();
                                 x++)
                            {
                                binaryMatrix->setAtPosition(line.getBegin().getY(), x, 0);
                            }
                        }
                    }
                    lines.clear();
                }

            }
        }
    }

    return binaryMatrix;
}

int meanHistogram(Matrix<double> channel, double totalPixels, int &maxPosition)
{
    double total = 0;
    maxPosition = -1;
    double maxValue = -1;
    for (int c = 0; c < channel.getCols(); c++)
    {
        double cValue = channel.getAtPosition(0, c);
        total += (c * cValue);
        if (cValue > maxValue)
        {
            maxValue = cValue;
            maxPosition = c;
        }
    }
    return total / totalPixels;
}

ptr_RGBMatrix colorThreshold(ptr_RGBMatrix rgbImage,
                             ptr_RGBMatrix colorHistogram)
{
    //ptr_RGBMatrix rgbImage = inputImage.getRGBMatrix();
    int rows = rgbImage->getRows();
    int cols = rgbImage->getCols();
    double totalPixels = rows * cols;

    //ptr_RGBMatrix colorHistogram = inputImage.getRGBHistogram(); // matrix with 1 row
    Matrix<double> redHistogram(1, colorHistogram->getCols(), 0);
    Matrix<double> greenHistogram(1, colorHistogram->getCols(), 0);
    Matrix<double> blueHistogram(1, colorHistogram->getCols(), 0);
    for (int c = 0; c < colorHistogram->getCols(); c++)
    {
        RGB color = colorHistogram->getAtPosition(0, c);
        redHistogram.setAtPosition(0, c, color.R);
        greenHistogram.setAtPosition(0, c, color.G);
        blueHistogram.setAtPosition(0, c, color.B);
    }
    int maxRed = -1, maxGreen = -1, maxBlue = -1;
    int meanRed = meanHistogram(redHistogram, totalPixels, maxRed);
    int meanGreen = meanHistogram(greenHistogram, totalPixels, maxGreen);
    int meanBlue = meanHistogram(blueHistogram, totalPixels, maxBlue);
    cout << "\nMean Red - Max Red: " << meanRed << "\t" << maxRed;
    cout << "\nMean Green - Max Green: " << meanGreen << "\t" << maxGreen;
    cout << "\nMean Blue - Max Blue: " << meanBlue << "\t" << maxBlue;

    int tRed = 0, tBlue = 0;
    int tGreen = meanGreen;
    tRed = maxRed - (maxRed * 0.2);
    tBlue = maxBlue - (maxBlue * 0.2);
    tGreen = maxGreen - (maxGreen * 0.2);
    cout << "\nThreshold: " << tRed << "\t" << tGreen << "\t" << tBlue << endl;
    RGB color;
    color.R = color.G = color.B = 0;
    ptr_RGBMatrix thresh = new Matrix<RGB>(rows, cols, color);
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            color = rgbImage->getAtPosition(r, c);
            if (color.R >= tRed)
                color.R = 0;
            if (color.G >= tGreen)
                color.G = 0;
            if (color.B >= tBlue)
                color.B = 0;
            thresh->setAtPosition(r, c, color);
        }
    }
    ptr_RGBMatrix result(rgbImage);
    RGB bck;
    bck.R = bck.G = bck.B = 0;
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            color = thresh->getAtPosition(r, c);
            if (color == 0)
                result->setAtPosition(r, c, bck);
        }
    }
    return result;
}

void showMatrix(ptr_IntMatrix intMatrix)
{
    int rows = intMatrix->getRows();
    int cols = intMatrix->getCols();
    cout << endl;
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            cout << intMatrix->getAtPosition(r,c) << " \t";
        }
        cout << endl;
    }
}

ptr_IntMatrix copyGrayMatrix(ptr_IntMatrix grayMatrix, int fromRow, int toRow, int fromCol, int toCol)
{
    int rows = toRow - fromRow;
    int cols = toCol- fromCol;
    ptr_IntMatrix result = new Matrix<int>(rows, cols);
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            int grayValue = grayMatrix->getAtPosition(r+fromRow,c+fromCol);
            result->setAtPosition(r,c,grayValue);
        }
    }
    return result;
}

ptr_RGBMatrix copyRGBMatrix(ptr_RGBMatrix RGBMatrix, int fromRow, int toRow, int fromCol, int toCol)
{
    int rows = toRow - fromRow;
    int cols = toCol- fromCol;
    ptr_RGBMatrix result = new Matrix<RGB>(rows, cols);
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            RGB color = RGBMatrix->getAtPosition(r+fromRow,c+fromCol);
            result->setAtPosition(r,c,color);
        }
    }
    return result;
}

void calculateHistogram(ptr_IntMatrix grayMatrix, ptr_IntMatrix grayHistogram, float &medianHistogram, float &meanHistogram, float &thresholdValue)
{
    if (grayMatrix->getCols() != 0)
    {
        float total = 0;
        float pi = 0;
        int array[BIN_SIZE] =
        { 0 };

        for (int r = 0; r < grayMatrix->getRows(); r++)
        {
            for (int c = 0; c < grayMatrix->getCols(); c++)
            {
                int k = grayMatrix->getAtPosition(r, c);
                array[k]++;
            }
        }

        for (int k = 0; k < BIN_SIZE; k++)
        {
            // save and calculate the mean of gray histogram
            grayHistogram->setAtPosition(0, k, array[k]);
            total += array[k];
            pi += (k * array[k]);
        }

        // calculate the mean of histogram
        meanHistogram = (pi / total);
        // calculate the median of histogram
        float avm = total / 2;
        float temp = 0;
        for (int m = 0; m < BIN_SIZE; m++)
        {
            temp += array[m];
            if (temp >= avm)
            {
                medianHistogram = m;
                break;
            }
        }
    }
}

int indexMaxInRangeOfGrayHistogram(ptr_IntMatrix grayHistogram, int from, int to)
{
    int indexMax = from, max = grayHistogram->getAtPosition(0, from);
    for (int i = from + 1 ; i < to; i++)
    {
        int temp = grayHistogram->getAtPosition(0, i);
        if (temp > max)
        {
            max = temp;
            indexMax = i;
        }
    }
    return indexMax;
}

int indexMinInRangeOfGrayHistogram(ptr_IntMatrix grayHistogram, int from, int to)
{
    int indexMin = from, min = grayHistogram->getAtPosition(0, from);
    for (int i = from + 1 ; i < to; i++)
    {
        int temp = grayHistogram->getAtPosition(0, i);
        if (temp < min)
        {
            min = temp;
            indexMin = i;
        }
    }
    return indexMin;
}

void calculateThreshold(float &medianHistogram, float &meanHistogram, float &thresholdValue, ptr_IntMatrix grayHistogram)
{
    // limit1 is the small value between meanHistogram and medianHistogram
    // then compare with 120 to choose a suitable of limit1
    int limit1 =
            meanHistogram < medianHistogram ? meanHistogram : medianHistogram;
    limit1 = (limit1 >= 120) ? (limit1 - 25) : (limit1 - 5);

    // limit1 is the big value between meanHistogram and medianHistogram
    int limit2 =
            meanHistogram > medianHistogram ? meanHistogram : medianHistogram;

    // find the first peak of gray histogram
    int indexMax1 = indexMaxInRangeOfGrayHistogram(grayHistogram, 0, limit1);

    // find the second peak of gray histogram
    int indexMax2 = indexMaxInRangeOfGrayHistogram(grayHistogram, limit2, BIN_SIZE);

    int indexMin = indexMinInRangeOfGrayHistogram(grayHistogram,indexMax1, limit2);

    cout << "indexMin:\t" << indexMin << endl;
    cout << "indexMax1:\t" << indexMax1 << endl;
    cout << "indexMax2:\t" << indexMax2 << endl;
    float mid1 = (indexMin + indexMax1) / 2;
    float mid2 = (indexMin + indexMax2) / 2;
    thresholdValue = (mid1 + mid2) / 2;
}

void copySmallToBigIntMatrix(ptr_IntMatrix intSmallMatrix, ptr_IntMatrix intBigMatrix, int fromRow, int fromCol)
{
    int rows = intSmallMatrix->getRows();
    int cols = intSmallMatrix->getCols();
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            int intValue = intSmallMatrix->getAtPosition(r,c);
            intBigMatrix->setAtPosition(r+fromRow,c+fromCol,intValue);
        }
    }
}
