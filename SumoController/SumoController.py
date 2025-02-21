import numpy as np
import threading
import nltk
import os

class TextColor:
    RED = '\033[31m'
    GREEN = '\033[32m'
    YELLOW = '\033[33m'
    BLUE = '\033[34m'
    PURPLE = '\033[35m'
    TEAL = '\033[36m'
    GREY = '\033[37m'
    #
    LIGHT_RED = '\033[91m'
    LIGHT_GREEN = '\033[92m'
    LIGHT_YELLOW = '\033[93m'
    LIGHT_BLUE = '\033[94m'
    LIGHT_PURPLE = '\033[95m'
    LIGHT_TEAL = '\033[96m'
    LIGHT_GREY = '\033[97m'
    #
    BLACK = '\033[8m'
    #
    RESET = '\033[m'
class HighlightColor:
    RED = '\033[41m'
    GREEN = '\033[42m'
    YELLOW = '\033[43m'
    BLUE = '\033[44m'
    PURPLE = '\033[45m'
    TEAL = '\033[46m'
    GREY = '\033[47m'
    #
    LIGHT_RED = '\033[101m'
    LIGHT_GREEN = '\033[102m'
    LIGHT_YELLOW = '\033[103m'
    LIGHT_BLUE = '\033[104m'
    LIGHT_PURPLE = '\033[105m'
    LIGHT_TEAL = '\033[106m'
    LIGHT_GREY = '\033[107m'
    #
    WHITE = '\033[7m'
    #
    RESET = '\033[m'
class TextModification:
    BOLD = '\033[1m'
    LIGHT = '\033[2m'
    ITALICS = '\033[3m'
    UNDERLINE = '\033[4m'
    STRIKETHROUGH = '\033[9m'
    DOUBLE_UNDERLINE = '\033[21m'
    #
    RESET = '\033[m'
class ConsoleModification:
    CLEARLINE = '\x1b[2K'
    CONSOLEUP = '\x1b[1A'



class SumoBot:
    name = "Unnamed"
    connected = True
    port = -1           #network Port
    angularVelocity = 0 #rad/s
    forwardVelocity = 0 #cm/s
    rightMotorDC = 0    #Duty Cycle
    leftMotorDC = 0     #Duty Cycle
    def __init__(self, name, port):
        self.name = name
        self.port = port
class DisplayManager:
    RobotArray = []
    ShowDebugValues = False
    Rows = 0
    Columns = 0
    NumOfRobots = 0
    #Spacing Variables
    TopOffset = 1
    BottomOffset = 1
    HeightOfDebugValues = 5
    # Alignment Constants
    bufferSpace = 0
    remainder = 0
    firstOffset = 0
    def __init__(self, ShowDebugValues, Rows, Columns, RobotArray):
        self.ShowDebugValues = ShowDebugValues
        self.Rows = Rows
        self.Columns = Columns
        self.NumOfRobots = len(RobotArray)
        self.RobotArray = RobotArray
    
    def PrintConnectivityRow(self, LeftOffset, LeftRobot, MiddleOffset, RightRobot):
        left =  LeftRobot.name  + ' ' + (HighlightColor.LIGHT_GREEN if LeftRobot.connected  else HighlightColor.RED) + '  ' + HighlightColor.RESET
        print((' '*LeftOffset) + "{:<35}".format(left), end='')
        if(RightRobot != None):
            right = RightRobot.name + ' ' + (HighlightColor.LIGHT_GREEN if RightRobot.connected else HighlightColor.RED) + '  ' + HighlightColor.RESET
            print((' '*MiddleOffset), end='')
            print("{:<35}".format(right))
        else:
            print()

    def PrintDebugRow(self, LeftOffset, LeftRobot, MiddleOffset, RightRobot):
        print(' '*LeftOffset + 'Port: ' + "{:<20}".format(LeftRobot.port),end='')
        if(RightRobot != None):
            print(' '*MiddleOffset + 'Port: ' + "{:<20}".format(RightRobot.port))
        else:
            print()

        print(' '*LeftOffset + 'Angular Velocity: ' + "{:8.3f}".format(LeftRobot.angularVelocity),end='')
        if(RightRobot != None):
            print(' '*MiddleOffset + 'Angular Velocity: ' + "{:8.3f}".format(RightRobot.angularVelocity))
        else:
            print()

        print(' '*LeftOffset + 'Forward Velocity: ' + "{:8.3f}".format(LeftRobot.forwardVelocity),end='')
        if(RightRobot != None):
            print(' '*MiddleOffset + 'Forward Velocity: ' + "{:8.3f}".format(RightRobot.forwardVelocity))
        else:
            print()

        print(' '*LeftOffset + 'Right DC:         ' + "{:8.3f}".format(LeftRobot.rightMotorDC),end='')
        if(RightRobot != None):
            print(' '*MiddleOffset + 'Right DC:         ' + "{:8.3f}".format(RightRobot.rightMotorDC))
        else:
            print()

        print(' '*LeftOffset + 'Left DC:          ' + "{:8.3f}".format(LeftRobot.leftMotorDC),end='')
        if(RightRobot != None):
            print(' '*MiddleOffset + 'Left DC:          ' + "{:8.3f}".format(RightRobot.leftMotorDC))
        else:
            print()

    def UpdateBlockAlignmentConstants(self):
        self.NumOfBlockRows = int(np.ceil(self.NumOfRobots/2))
        
        ActiveRows = self.Rows - self.TopOffset - self.BottomOffset
        BlockHeight = (self.HeightOfDebugValues + 1) if self.ShowDebugValues else 1
        self.bufferSpace = int(np.floor( (ActiveRows - self.NumOfBlockRows*BlockHeight) / (self.NumOfBlockRows+1) ))
        self.remainder = ActiveRows - self.NumOfBlockRows*self.bufferSpace-self.NumOfBlockRows*BlockHeight
        self.firstOffset = 0
        if(self.remainder > 2*self.bufferSpace):
            self.firstOffset = 1
            self.remainder = self.remainder - 1

    def InitalizeDisplay(self):
        # Update Vars
        self.UpdateBlockAlignmentConstants()
        # Modify Array
        if(len(self.RobotArray) < 2*self.NumOfBlockRows):
            ModifiedArray = self.RobotArray + [None]
        else:
            ModifiedArray = self.RobotArray
        # Print page
        print("/help for a list of commands")
        for i in range(self.NumOfBlockRows):
            # symetry modification
            if(i == 0 and self.firstOffset == 1):
                print("\n",end='')

            #whitespace buffer
            print("\n"*self.bufferSpace,end='')

            #connectivity
            self.PrintConnectivityRow(8, ModifiedArray[2*i], 8, ModifiedArray[2*i+1])

            #Debug
            if(self.ShowDebugValues == True):
                self.PrintDebugRow(8, ModifiedArray[2*i], 8, ModifiedArray[2*i+1])

            #Controllers
        print("\n"*self.remainder,end='')

    #def UpdateValues():
class ProgramManager:
    Robots = [SumoBot("Robot 1", 1234), SumoBot("Robot 2", 2345),
              SumoBot("Robot 3", 3456), SumoBot("Robot 4", 4567),
              SumoBot("Robot 5", 5678)]#, SumoBot("Robot 6", 6789)]
    DM = DisplayManager(True, 29, 40, Robots)
    
    def parseCommand(self, command):
        tokens = nltk.word_tokenize(command)
        if(tokens[0] == 'debug' and len(tokens) == 2):
            if(tokens[1].lower() == 'true'):
                self.DM.ShowDebugValues = True
                os.system('cls')
                self.DM.InitalizeDisplay()
            elif(tokens[1].lower() == 'false'):
                self.DM.ShowDebugValues = False
                os.system('cls')
                self.DM.InitalizeDisplay()

    def consoleLoop(self):
        userInput = ""
        while(not(userInput.lower() == 'quit' or userInput.lower() == 'q')):
            userInput = input('')
            print(ConsoleModification.CONSOLEUP + ConsoleModification.CLEARLINE, end='')
            self.parseCommand(userInput)

    def start(self):
        self.DM.InitalizeDisplay()
        console_thread = threading.Thread(target=self.consoleLoop())
        #start threads
        console_thread.start()
        #join threads
        console_thread.join()

"""
Communication with the Sumo-Bots will be over UDP, we will haave to do this off of our own router because the
school will not be happy with us trying to open new ports.
Every robot will have it's own port to communicate on. This is saved in the SumoBot class
There is also a display class that handles the UI. Additionally, a config file is present that allows you to
input the amount of robots you have and the properties of those robots.

Human Input Devices (HIDs) will have their own class to handle them. And 

Each robot will have a commanded angular velocity and a forward/backward speed. Those will translate into motor
Duty Cycles (DCs).
"""

PM = ProgramManager()

PM.start()