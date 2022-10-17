#include "../headers/parse_args.h"

#include <stdio.h>
#include <string.h>

ARGS argsParser(size_t argSize, char* args[])
{    
    // init data
    ARGS data;

    for (size_t i=0; i<NB_ARGS_FLAGS; i++)
    {
        data.args[i].nb = 0;
        data.args[i].args = NULL;
        data.allocate[i] = 0;
    }

    ArgsFlags currentFlag = ARG_InputFile;
    ARG*  currentARG = &(data.args[ARG_InputFile]);
    char* currentText;

    // read and parse args
    for (size_t i=0; i<argSize; i++) 
    {
        currentText = args[i];

        // check the flags
        if (currentText[0] == '-'){
            if (strlen(currentText) > 2)
                ERROR("For the time beeing the compiler support only one flag after a -")

            if (currentText[1] == 'i'){
                if (data.allocate[ARG_InputFile])
                    ERROR("Multiple input flags")

                currentFlag = ARG_InputFile;

            } else if (currentText[1] == 'o') {
                if (data.allocate[ARG_OutputFile])
                    ERROR("Mutltiple output flags")

                currentFlag = ARG_OutputFile;

            } else {
                ERROR("-%c is an Unknown flag", currentText[1])
            }
            currentARG = &(data.args[currentFlag]);

        } else {
            // extract the data
            currentARG->nb++;
            currentARG->args = (char**)realloc(currentARG->args, sizeof(char*)*currentARG->nb);
            
            CHECK_ALLOCATE(currentARG->args, "Unable to allocate memory to store args")
            currentARG->args[currentARG->nb - 1] = currentText;
            
            data.allocate[currentFlag] = 1;
        }
    }

    // validate the data
    if (!data.allocate[ARG_InputFile])
        ERROR("No input file")

    if (!data.allocate[ARG_OutputFile])
    {
        data.args[ARG_OutputFile].nb = 1;
        data.args[ARG_OutputFile].args = (char**)malloc(sizeof(char));
        CHECK_ALLOCATE(data.args[ARG_OutputFile].args, "Unable to allocate the output path")
        data.args[ARG_OutputFile].args[0] = "main.exe";
    }

    return data;
}





