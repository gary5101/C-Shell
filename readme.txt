Commands to execute shell
    $ make
    $ ./a.out

    (a.out can be executed from anywhere)



#main.c
    -->prompting the user
        ->getting the username and systemname;
        ->changes control to executable files directory and considers it a home;

    -->parsing the input string
        FUNCTIONS USED: space();
        ->removes mutliple spaces and tabs;
        ->breaks the command into chucks seperated by semicolon;
        ->removes waste spaces;
    
    -->reading the command given and redirecting it

    -->after the process is executed reprompt the user

    note:-
        ->handles commands below the size of 1000 only(including spaces and tabs);


#cd.c
    -->redirected to when any cd command is given

    -->changes control to the directory given by the user using chdir()

    note:-
        ->handles '~' also

#pwd.c
    -->prints the absolute path of current directory using getpid()


#echo.c
    -->directly prints the words written after echo;

    --> case where no quotes are given is only handled;


#ls.c
    -->searches through user's input for the flags given

    -->handles only -a and -l flags

    -->considers all the not flag strings as directories

    -->for each directory all the files are read and prints according to the flags given

    -->  -a
        -> reads all the files and prints even the hidden files

    -->  -l
        FUNCTIONS USED: lsl()
        -> function takes in the file name;
        ->gets all the stats required using "struct stat";
        ->prints them in required format;

    note:-
        ->handles '~' also;
        ->handles cases where flag is invokes mutliple times;
        ->order of flags and directories don't matter;


#pinfo.c
    -->takes in two cases
        ->when process id is given
            --prints the process status, virtual memory and executable path of that process
        ->when no id is given
            -- prints the process id ,process status, virtual memory and executable path of that process


#rest.c
    -->non built-in commands are executed

    -->execvp() executes all the other commands

    -->handles foreground and background processes seperately

    -->prompts the user on exiting a background process
    
    -->foreground processes stop the terminal

    -->jobs ,kjob ,bg, fg, overkill are also implemented
        FUNCTIONS USED: jobs(), kjob(), bg(),fg(),overkill().


    -->terminal ignores ctrl +c, ctrl+z, ctrl+d and other such signals
        FUNCTIONS USED: handle_int(), signalstoph()



#history.c
    -->stores all the commands executed in a file
    FUNCTIONS USED: history_store()
    
    -->prints the history of command
    FUNCTIONS USED: history_print()
        ->prints last n lines 
        ->or prints last 20 lines


#redirc.c
    -->handles all piping and redirection cases

    -->the general command is of the format [command 1<filename1|command 2|command 3|.....|command n>filename2].
       Here all the redirctions and pipes are optional.
       