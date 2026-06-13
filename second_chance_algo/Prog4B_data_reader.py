import openpyxl, re

# HOW TO USE THIS SCRIPT:
# 0a. Make sure you have Python installed
# 0b. install openpyxl with the following command: pip install openpyxl
# 1. Obtain a text file containing data from program 4b
# 2. pass the file name into the dataFile variable
# 3. create an Excel spreadsheet and pass the file name to spreadSheetName.
# 4. run the program.
# 5. Make graphs at your leisure (I did not implement that functionality here)


# to user: change names of files
dataFile = open("output.txt")
# spreadSheetName = "Execution Data and Discussion.xlsx"
spreadSheetName = "test.xlsx"
wb = openpyxl.load_workbook(str(spreadSheetName))


# Regexes

secondsPattern = re.compile(r'(0.(\d)+)')

# Test random access(cache enabled): 0.021778 sec
randomAccessEnabledPattern = re.compile(r'Test random access\(cache enabled\): 0.(\d)+ sec')
randomAccessDisabledPattern = re.compile(r'Test random access\(cache disabled\): 0.(\d)+ sec')

localizedAccessEnabledPattern = re.compile(r'Test localized access\(cache enabled\): 0.(\d)+ sec')
localizedAccessDisabledPattern = re.compile(r'Test localized access\(cache disabled\): 0.(\d)+ sec')

mixedAccessEnabledPattern = re.compile(r'Test mixed access\(cache enabled\): 0.(\d)+ sec')
mixedAccessDisabledPattern = re.compile(r'Test mixed access\(cache disabled\): 0.(\d)+ sec')



print(wb.get_sheet_names())

sheet = wb.get_sheet_by_name(wb.get_sheet_names()[0])

# lists
randAccessEnabledList = []
randAccessDisabledList = []
localizedAccessEnabledList = []
localizedAccessDisabledList = []
mixedAccessEnabledList=[]
mixedAccessDisabledList=[]


# Time lists
randAccessEnabledTimeList = []
randAccessDisabledTimeList = []
localizedAccessEnabledTimeList = []
localizedAccessDisabledTimeList = []
mixedAccessEnabledTimeList=[]
mixedAccessDisabledTimeList=[]

def readPatternInLine(line, pattern, list):
    for match in re.finditer(pattern,line):
        # print(match.group())
        list.append(match.group())

def readTimesInList(list):
    timeList = []
    for line in list:
        timeList.append(re.search(secondsPattern,line).group(0))
    return timeList

def writeToSpreadsheet(sheet, list, startRow, column):
    rowNum = startRow
    for item in list:
        sheet.cell(row=rowNum,column=column).value = float(item)
        rowNum += 1


for i, line in enumerate(dataFile):
    readPatternInLine(line, randomAccessEnabledPattern,randAccessEnabledList)
    readPatternInLine(line,randomAccessDisabledPattern,randAccessDisabledList)
    readPatternInLine(line,localizedAccessEnabledPattern,localizedAccessEnabledList)
    readPatternInLine(line,localizedAccessDisabledPattern,localizedAccessDisabledList)
    readPatternInLine(line,mixedAccessEnabledPattern,mixedAccessEnabledList)
    readPatternInLine(line,mixedAccessDisabledPattern,mixedAccessDisabledList)

randAccessEnabledTimeList = readTimesInList(randAccessEnabledList)
randAccessDisabledTimeList = readTimesInList(randAccessDisabledList)
localizedAccessEnabledTimeList = readTimesInList(localizedAccessEnabledList)
localizedAccessDisabledTimeList = readTimesInList(localizedAccessDisabledList)
mixedAccessEnabledTimeList= readTimesInList(mixedAccessEnabledList)
mixedAccessDisabledTimeList= readTimesInList(mixedAccessDisabledList)

writeToSpreadsheet(sheet,randAccessEnabledTimeList,3,2)
writeToSpreadsheet(sheet,randAccessDisabledTimeList,3,5)
writeToSpreadsheet(sheet,localizedAccessEnabledTimeList,3,3)
writeToSpreadsheet(sheet,localizedAccessDisabledTimeList,3,6)
writeToSpreadsheet(sheet,mixedAccessEnabledTimeList,3,4)
writeToSpreadsheet(sheet,mixedAccessDisabledTimeList,3,7)


wb.save(str(spreadSheetName))

