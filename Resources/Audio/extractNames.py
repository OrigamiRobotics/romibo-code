import csv
import os
import shutil

#REFAC: only works on clean run; short file names directory should be empty

audioFilesPath = "/Users/QoLT/Documents/Projects/romibo-code/Resources/Audio/"
shortFilePath = "/Users/QoLT/Documents/Projects/romibo-code/Resources/Audio/Short File Names/"

allFilesNames = [];
allButtonNames = [];
shortFileNames = [];
toCSV = [];

def shortFileExists(fileName):
    
    return os.path.exists(os.path.join(shortFilePath, fileName + ".wav"))


def listFiles(path):



        for file in os.listdir(path):
            print('File: {0}'.format(file))
            #allFilesNames.append(file)

            filelength = len(file);

            if (file[filelength-4:filelength] != ".wav"):
                continue;
            
            buttonname = file[0:filelength-4] #removes .wav
            buttonname = buttonname.replace("_", " ")#removes underscore
            buttonText = buttonname.capitalize()
            #print("Button text: {0}".format(buttonText))
            
            allButtonNames.append(buttonText)

            if (filelength > 12):
                print("File name too long: {0}".format(file))

                newFileName = file[0:8].replace(" ", "_");

                if (shortFileExists(newFileName)):

                    for i in range(0,100):

                        newFileName = file[0:6]
                        newFileName += str(i)

                        if (not(shortFileExists(newFileName))):
                            print("Found workable file name: {0}".format(newFileName))
                            break;

                    if (shortFileExists(newFileName)):
                        print("FAIL (too many sounds with this prefix): {0}".format(newFileName))
        
                newFileName += ".wav"
                print("New file name: {0}".format(newFileName))

                newPath = os.path.join(shortFilePath, newFileName)
                shortFileNames.append(newFileName)

            else:
                newFileName = file.replace(" ", "_")
                newPath = os.path.join(shortFilePath, newFileName)
                shortFileNames.append(newFileName)


            if (os.path.exists(newPath)):
                continue;

            shutil.copyfile(os.path.join(path, file), newPath)

                

  
        


for root, dirs, files in os.walk(audioFilesPath):

    for dirName in dirs:
        print("Directory: {0}".format(dirName))
        if (dirName == "Short File Names"):
            continue
            
        listFiles(dirName)


#toCSV = zip(allFilesNames, allButtonNames)
toCSV = zip(shortFileNames, allButtonNames)

with open(os.path.join(shortFilePath,'audio files short names.csv'), 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerows(toCSV)    


print ("OK")
