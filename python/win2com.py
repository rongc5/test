import win32com.client, os
from win32com.client import constants

f = 'test1.xls'
xlsPath = f
absPath = os.path.abspath(xlsPath);
#absPath= '''r"%s"''' % (absPath)
print absPath
DELETE_THIS = "X"

exc = win32com.client.gencache.EnsureDispatch('Excel.Application')
exc.Visible = 1
exc.Workbooks.Open(absPath)


row = 1
while True:
    exc.Range("B%d" % row).Select()
    data = exc.ActiveCell.FormulaR1C1
    exc.Range("A%d" % row).Select()
    condition = exc.ActiveCell.FormulaR1C1

    if data == '':
        break
    elif condition == DELETE_THIS:
        exc.Rows("%d:%d" % (row, row)).Select()
        exc.Selection.Delete(Shift=constants.xlUp)
    else:
        row += 1