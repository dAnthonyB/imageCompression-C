//Dennis Bruce
//Assignment 4
#include <stdio.h>
#include <stdlib.h>
void printImage(int array[], int height, int width, int maxScale, char *imageName){
    FILE *fp;
    //creates a new image file with header using the values from above
    fp = fopen(imageName, "w+");
    fprintf(fp, "P2\n%i  %i\n%i\n", height, width, maxScale);
    //writes pixel grayscale values from average into new image
    int r,c;
    for (r = 0; r < height; r++){
        for (c = 0; c < width; c++)
            fprintf(fp, "%d ", array[r*width+c]);
    }
    printf("%s has been created\n", imageName);
    fclose(fp);
}
int encoder(int value, int maxScale, int levels){
    if(value == maxScale)
        return levels-1;
    else{
        float delta = maxScale/levels;
        return value/delta;
    }
}
int decoder(int value, int maxScale, int levels){
    float delta = maxScale/levels;
    return (value*delta)+(delta/2);
}
float findDistortion(int orig[], int new[], int length){
    float total = 0;
    int r;
    for(r = 0; r < length; r++){
        total += (orig[r] - new[r])*(orig[r] - new[r]);
    }
    return total/r;
}
main (int argc, char *argv[]) {
    //grabs arguments from command-line
    char *imageName;
    if(argc > 1)
        imageName = argv[2];
    else
        imageName = "baboon.pgma";

    //opens the original image
    FILE *fp;
    fp = fopen(imageName, "r");
    
    //gets the header out of the way and saves values from pgma file
    char temp[3];
    do{
        fscanf(fp, "%s", temp);
    }while(atoi(temp) < 512);
    int height = atoi(temp);
    fscanf(fp, "%s", temp);
    int width = atoi(temp);
    fscanf(fp, "%s", temp);
    int maxScale = atoi(temp);
    int area = height*width;
    int origImage[area];
    
    //stores pixels' grayscale values
    int r, c;
    for (r = 0; r < height; r++){
        for (c = 0; c < width; c++){
            fscanf(fp, "%s", temp);
            origImage[r*width+c] = atoi(temp);
        }
    }
    fclose(fp);
    
    int encoded10Scale[area];
    int numScaleLevels = 10;
    for (r = 0; r < area; r++)
        encoded10Scale[r] = encoder(origImage[r], maxScale, numScaleLevels);
    printImage(encoded10Scale,height,width,numScaleLevels-1,"encoded10Scale.pgma");
    
    int decoded10Scale[area];
    for (r = 0; r < area; r++)
        decoded10Scale[r] = decoder(encoded10Scale[r], maxScale, numScaleLevels);
    printImage(decoded10Scale,height,width,maxScale,"decoded10Scale.pgma");
    
    int avgDiff10Scale[area];
    int max = 0;
    for (r = 0; r < area; r++){
        avgDiff10Scale[r] = abs(origImage[r]-decoded10Scale[r]);
        if(avgDiff10Scale[r] > max)
            max = avgDiff10Scale[r];
    }
    printImage(avgDiff10Scale,height,width,max,"errorImage10Scale.pgma");
    float distortion;
    distortion = findDistortion(origImage, decoded10Scale, area);
    printf("for 10-gray levels, MSE = %f\n\n",distortion);
    
    int encoded2Scale[area];
    numScaleLevels = 2;
    for (r = 0; r < area; r++)
        encoded2Scale[r] = encoder(origImage[r], maxScale, numScaleLevels);
    printImage(encoded2Scale,height,width,numScaleLevels-1,"encoded2Scale.pgma");
    
    int decoded2Scale[area];
    for (r = 0; r < area; r++)
        decoded2Scale[r] = decoder(encoded2Scale[r], maxScale, numScaleLevels);
    printImage(decoded2Scale,height,width,maxScale,"decoded2Scale.pgma");
    
    int avgDiff2Scale[area];
    max = 0;
    for (r = 0; r < area; r++){
        avgDiff2Scale[r] = abs(origImage[r]-decoded2Scale[r]);
        if(avgDiff2Scale[r] > max)
            max = avgDiff10Scale[r];
    }
    printImage(avgDiff2Scale,height,width,max,"errorImage2Scale.pgma");
    
    distortion = findDistortion(origImage, decoded2Scale, area);
    printf("for 2-gray levels, MSE = %f\n\n", distortion);
}