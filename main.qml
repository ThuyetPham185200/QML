import QtQuick 2.9
import QtQuick.Window 2.1
//import QtQuick.Dialogs 1.0
import QtQuick.Controls 1.0
import thuyet.loadfile.com 1.0
import thuyet.connector.com 1.0
import thuyet.receiver.com 1.0

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

//    InputPanel {
//        id: inputPanel
//        z: 99
//        x: 0
//        y: window.height
//        width: window.width

//        states: State {
//            name: "visible"
//            when: inputPanel.active
//            PropertyChanges {
//                target: inputPanel
//                y: window.height - inputPanel.height
//            }
//        }
//        transitions: Transition {
//            from: ""
//            to: "visible"
//            reversible: true
//            ParallelAnimation {
//                NumberAnimation {
//                    properties: "y"
//                    duration: 250
//                    easing.type: Easing.InOutQuad
//                }
//            }
//        }
//    }

    LoadFile {
        id: loadfile
    }

    UDPConnector {
        id: connector
    }

    Receiver {
        id: receiver
    }

//    FileDialog {
//        id: fileDialog
//        title: "Please choose a file"
//        folder: shortcuts.home
//        nameFilters: [ "Image files (*.jpg *.png)", "All files (*)" ]
//        onAccepted: {
//            console.log("You chose: " + fileDialog.fileUrls)
//            fileDialog.close()
//            textField.text = fileDialog.fileUrl
//        }
//        onRejected: {
//            console.log("Canceled")
//        }
//    }

    TextField {
        id: textGroupAddr
        x: 113
        y: 168
        width: 284
        height: 40
        text: qsTr("239.255.43.21")
    }
    TextField {
        id: textPort
        x: 113
        y: 108
        width: 284
        height: 40
        text: qsTr("45454")
    }
    TextField {
        id: textData
        x: 113
        y: 228
        width: 284
        height: 40
        text: qsTr("Show data")
    }
    Button {
        id: choosebtn
        x: 447
        y: 168
        text: qsTr("Chosen")
        onClicked: {
            receiver.getInfor(textGroupAddr.text, textPort.text);
        }
    }

    Button {
        id: uploadbtn
        x: 447
        y: 108
        text: qsTr("Start")
        onClicked: {
//          console.log("upload file " + textField.text)
            //textData.text = receiver.processPendingDatagrams()

        }
    }
}
