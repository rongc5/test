import win32com.client,  pythoncom
from win32com.client import constants, Dispatch


f = "test1.xls"
DELETE_THIS = "X"

#exc = win32com.client.Dispatch("Excel.Application")
exc = win32com.client.gencache.EnsureDispatch("Excel.Application")

exc.Visible = 1
exc.Workbooks.Open(Filename=f)

wbk = exc.ActiveWorkbook
sheet = wbk.Sheets(2)
exc.ActiveWorkbook.Sheets(2)

wbk.SaveAs()
i = 1
while i < 21:
    data = sheet.Cells(i,1).Value
    print data
    #if not data:
    #   break
    if data == DELETE_THIS:
        sheet.Rows(i).Delete(Shift=constants.xlUp)
    else:
        i += 1

exc.ActiveWorkbook.SaveAs(f)


