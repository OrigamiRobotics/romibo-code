import csv
import os
import shutil

#REFAC: only works on clean run; short file names directory should be empty

audioFilesPath = "/Users/QoLT/Documents/Projects/romibo-code/Resources/Audio/"
RBFilePath = "/Users/QoLT/Documents/Projects/romibo-code/Resources/Audio/RB Prefix/"

allFilesNames = [];
allButtonNames = [];
shortFileNames = [];
toCSV = [];

def shortFileExists(fileName):
    
    return os.path.exists(os.path.join(shortFilePath, fileName + ".wav"))


def listFiles(path):

    for file in os.listdir(path):
        print('File: {0}'.format(file))
        
        filelength = len(file);


        if (file[filelength-4:filelength] != ".wav"):
            continue;

        newFileName = 'RB_{0}'.format(file)
        newFileName = newFileName.replace('.', ' ').replace(' ', "_")

        print(newFileName)

        newFileLength = len(newFileName)
        newFileName = '{0}.wav'.format(newFileName[:-4])

        print(newFileName)

        newPath = os.path.join(RBFilePath, newFileName);
            
        if (os.path.exists(newPath)):
            continue;

        shutil.copyfile(os.path.join(path, file), newPath)

                


for root, dirs, files in os.walk(audioFilesPath):

    for dirName in dirs:
        print("Directory: {0}".format(dirName))
        if ((dirName == "Short File Names") or (dirName == "RB Prefix")):
            continue
            
        listFiles(dirName)

print ("OK")
