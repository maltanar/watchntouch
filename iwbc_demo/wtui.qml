import Qt 4.7

Rectangle {

    id: window
    z:-1
    width: 800
    //height: 300
    y: 0
    color: "transparent"
    property string activeFunction: ""

    Row{                //BOTTOM MENU
        id: bottomMenu
        y: window.height - bottomMenu.height
        spacing: window.width/140.0

        Rectangle {      //NOTIFICATION BUTTON
            id: rectNotification
            color: "#edec99";
            width: window.width/17       //TODO: DAHA INCE OLSUN
            height: (width/60)*137
            Image{
                anchors.centerIn: parent
                width: window.width/44.52; height: window.width/11.63    // CALCULATION: width: window.width/ (1024/ImageWidth)
                fillMode: Image.PreserveAspectFit
                smooth: true
                source: "images/mainmenu/notificationOff.png" //TODO: NOTIFICATION ON/OFF
            }
        }

        Rectangle {      // RECORD BUTTON
            id: rectRec
            color: "#e199aa";
            width: window.width/17
            height: (width/60)*137
            Image{
                id: recId
                anchors.centerIn: parent
                width: window.width/21.33; height: window.width/19.3    // CALCULATION: width: window.width/ (1024/ImageWidth)
                fillMode: Image.PreserveAspectFit
                smooth: true
                source: "images/mainmenu/recOff.png" //TODO: NOTIFICATION ON/OFF

                MouseArea {                    
                    anchors.fill: parent
                    property int recordNo: 0
                    onClicked: {
                        recordNo = recordNo+1;
                        if (recordNo%2==0)
                            recId.source = "images/mainmenu/recOn.png";
                        if (recordNo%2==1)
                            recId.source = "images/mainmenu/recOff.png";
                    }
                }
            }

        }

        Row {           // FUNCTION BUTTONS
            //anchors.bottom: window.bottom
            spacing: 0

            Rectangle {                     // COLLABORATION BUTTON
                id: rectCollaboration
                color: "#de9ce4"
                width: window.width/7.11
                height: (width/144)*137
                Image{
                    anchors.top: rectCollaboration.top
                    anchors.topMargin: window.width/68.2
                    anchors.horizontalCenter: rectCollaboration.horizontalCenter
                    width: window.width/12.8; height: window.width/11.77    // CALCULATION: width: window.width/ (1024/ImageWidth)
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/mainmenu/collaboration.png"
                }
                Text{
                    anchors.bottom: rectCollaboration.bottom
                    anchors.horizontalCenter: rectCollaboration.horizontalCenter
                    text: "collaboration"
                    font.family: "Ubuntu"
                    font.pointSize: window.width/68
                    color: "black"
                }

                Image{
                    id: textCollBackgroundCircle
                    visible: false
                    x: window.width/11.37
                    y: window.width/204.8
                    //anchors.horizontalCenter: rectCollaboration.horizontalCenter
                    width: window.width/21.33; height: window.width/19.32    // CALCULATION: width: window.width/ (1024/ImageWidth)
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/mainmenu/noBackground.png"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        window.activeFunction = "collaboration"
                        textColl.no=textColl.no+1;
                        if(textColl.no==0){
                            textCollBackgroundCircle.visible = false;
                            textColl.visible = false;
                        }
                        else{
                            textCollBackgroundCircle.visible = true;
                            textColl.visible = true;
                            activeCollaboration.visible = true;
                            activeSketch.visible = false;
                            activePresentation.visible = false;
                            activeWeb.visible = false;
                            activeMultimedia.visible = false;
                            activeConfig.visible = false;
                        }

                    }
                }
                Text{
                    id: textColl
                    visible: false
                    anchors.centerIn: textCollBackgroundCircle
                    property int no: 0
                    text: no
                    font.family: "Ubuntu"
                    font.pointSize: window.width/45
                    color: "black"
                }
                Rectangle{
                    id: activeCollaboration
                    visible: false;
                    anchors.bottom: parent.top
                    color: "#a900bd"
                    width: window.width/7.11
                    height: (width/30)
                }

                //DONGERI
            }

            Rectangle {                     // SKETCH BUTTON
                id: rectSketch
                property bool isMenuOn: false
                color: "#f9d299"
                width: window.width/7.11
                height: (width/144)*137
                Image{
                    anchors.top: rectSketch.top
                    anchors.topMargin: window.width/68.2
                    anchors.horizontalCenter: rectSketch.horizontalCenter
                    width: window.width/8.98; height: window.width/14.2;         // CALCULATION: width: window.width/ (1024/ImageWidth)
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/mainmenu/sketch.png"
                }
                Text{
                    anchors.bottom: rectSketch.bottom
                    anchors.horizontalCenter: rectSketch.horizontalCenter
                    text: "sketch"
                    font.family: "Ubuntu"
                    font.pointSize: window.width/68
                    color: "black"
                }
                Image{
                    id: textSketchBackgroundCircle
                    visible: false
                    x: window.width/11.37
                    y: window.width/204.8
                    //anchors.horizontalCenter: rectCollaboration.horizontalCenter
                    width: window.width/21.33; height: window.width/19.32    // CALCULATION: width: window.width/ (1024/ImageWidth)
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/mainmenu/noBackground.png"
                }
                MouseArea {
                    id: sketchButtonMouseArea
                    anchors.fill: parent


                    onClicked: {
                        if(rectSketch.isMenuOn){
                        sketchMenuHide.running = true;
                        //sketchButtonMenuHide.running = true;
                        //sketchButtonHide.running = true;
                        rectSketch.isMenuOn = false;

                        }
                        else{
                        sketchMenuReveal.running = true;
                        //sketchButtonMenuReveal.running = true;
                        //sketchButtonReveal.running = true;
                        rectSketch.isMenuOn = true;
                        }

                        //window.state = "addButtonReveal"
                        /*
                        if(acik birseyler varsa)
                                             onlari dikey sirala

                                        yeni butonunu goster

                                        */
                        window.activeFunction = "sketch"
                        textSketch.no=textSketch.no+1;

                        if(listModelSketch.count>0){
                        sketchButtonMenu.height = (window.width/8) * listModelSketch.count + window.width/100
                        }
                        else{
                        sketchButtonMenu.height = 0
                        }

                        if(textSketch.no==0){
                            textSketchBackgroundCircle.visible = false;
                            textSketch.visible = false;
                        }
                        else{
                            textSketchBackgroundCircle.visible = true;
                            textSketch.visible = true;
                            activeCollaboration.visible = false;
                            activeSketch.visible = true;
                            activePresentation.visible = false;
                            activeWeb.visible = false;
                            activeMultimedia.visible = false;
                            activeConfig.visible = false;
                        }
                    }
                }

                /*states: State {         //TODO: add button dokununca acilsin
                    name: "addButtonReveal"
                    when: !sketchButtonMouseArea.
                    //PropertyChanges { target: contextMenu; x: 0; open: true }
                    //PropertyChanges { target: mainView; x: 130 }
                    PropertyChanges { target: addButtonSketchRect; opacity: 1 }
                }

                transitions: Transition {
                    NumberAnimation { properties: "x,opacity"; duration: 300; easing.type: Easing.OutQuint }
                }*/

                Text{
                    id: textSketch
                    visible: false
                    anchors.centerIn: textSketchBackgroundCircle
                    property int no: 0
                    text: no
                    font.family: "Ubuntu"
                    font.pointSize: window.width/45
                    color: "black"
                }
                Rectangle{
                    id: activeSketch
                    visible: false;
                    anchors.bottom: parent.top
                    color: "#f18e00"
                    width: window.width/7.11
                    height: (width/30)
                }


                Rectangle{
                    id: sketchButtonMenu
                    color: "#f18e00"
                    opacity: 0
                    anchors.bottom: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width + window.width/34.13
                    height: window.width/100 // TODO: List'te uc tane var

                    //height: sketchListView.height + window.width/5

                    /*
                    states: State {
                             name: "moved"; when: mouseArea.pressed
                             PropertyChanges { target: rect; x: 50; y: 50 }
                         }

                    transitions: Transition {
                             NumberAnimation { properties: "x,y"; easing.type: Easing.InOutQuad }
                    */

                    VisualDataModel {               //TODO: ListElement'ten File system'e gecir
                        id: visualModel
                        model: ListModel {
                            id: listModelSketch
                            ListElement { name: "0";  file: "images/ske1.png" }
                            ListElement { name: "1";  file: "images/ske2.png" }
                            ListElement { name: "2";  file: "images/ske3.png" }
                        }

                        delegate: Rectangle {
                            height: window.width/8
                            width: window.width/6
                            color: "transparent"
                            //color: model.modelData.color
                            //Text { text: name }

                            Image{
                                id: name
                                width: window.width/7
                                height: width //onemli degil, aspect ratio korunuyor
                                source: file
                                anchors.centerIn: parent
                                visible: true
                                fillMode: Image.PreserveAspectFit
                                smooth: true
                            }

                                Image{
                                    //x: window.width/
                                    anchors.right: parent.right
                                    width: window.width/18.96; height: window.width/18.28   // CALCULATION: width: window.width/ (1024/ImageWidth)
                                    fillMode: Image.PreserveAspectFit
                                    smooth: true
                                    source: "images/mainmenu/menuClose3.png"
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {            // TODO: AIT OLAN PENCEREYI KAPATACAK
                                            listModelSketch.remove(index);
                                            if(listModelSketch.count>0){
                                            sketchButtonMenu.height = (window.width/8) * listModelSketch.count + window.width/100;
                                            }
                                            else{
                                            sketchButtonMenu.height = 0;
                                            }
                                        }
                                    }
                                }


                        }

                    }


                    ListView {              // TODO: butonun altina giriyor, bug var
                        id: sketchListView
                        anchors.fill: parent
                        model: visualModel
                    }
                    /*
                    Column {
                        id: sketchColumn
                        spacing: 2
                        add: Transition {
                            PropertyAnimation {
                                id: addSketchAnimation
                                target: sketchButtonMenu
                                property: "y"
                                to: sketchButtonMenu.y + 100
                                duration: 300
                            }
                        }
                        move: Transition {
                            NumberAnimation {
                                properties: "y"
                                easing.type: Easing.OutBounce
                            }
                        }

                        Rectangle{                          //SKETCH ELEMENT 1
                            width: window.width/6
                            height: window.width/9
                            color:"red"     //TODO: KALDIR

                            Image{
                                id: skeImg
                                anchors.centerIn: parent
                                visible: true
                                fillMode: Image.PreserveAspectFit
                                smooth: true
                                source: "images/ske.png"
                            }

                        }

                    }


*/
                }



                Rectangle{  // sol kanat
                    id: sketchLeftWing
                    opacity: 0

                    color: "#f18e00"
                    anchors.right: parent.left
                    anchors.top: activeSketch.top
                    width: window.width/65
                    height: parent.height + activeSketch.height
                    //y: (window.width/8) * 3 + window.width/100 //TODO: yukariya bak
                }

                Rectangle{  // sag kanat
                    id: sketchRightWing
                    opacity: 0
                    color: "#f18e00"
                    anchors.left: parent.right
                    anchors.top: activeSketch.top
                    width: window.width/70
                    height: parent.height + activeSketch.height
                    //y: (window.width/8) * 3 + window.width/100 //TODO: yukariya bak
                }


                Rectangle{
                    id: addButtonSketchRect
                    color:"transparent"
                    opacity: 0
                    //y: (window.width/8) * 3 + window.width/100 //TODO: yukariya bak
                    //x: window.width/72

                    Image{
                        id: addButtonSketchImg
                        x: -window.width/60
                        y: -window.width/60
                        //anchors.horizontalCenter: rectCollaboration.horizontalCenter
                        width: window.width/18.96; height: window.width/18.28   // CALCULATION: width: window.width/ (1024/ImageWidth)
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        source: "images/mainmenu/menuNew3.png"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {            //YENI SKETCH MENU ACACAK

                        }

                    }

                }

                PropertyAnimation { id: sketchMenuReveal; targets: [sketchButtonMenu, addButtonSketchRect, sketchLeftWing, sketchRightWing]; property: "opacity"; to: 1; duration: 300 }
                PropertyAnimation { id: sketchMenuHide; targets: [sketchButtonMenu, addButtonSketchRect, sketchLeftWing, sketchRightWing]; property: "opacity"; to: 0; duration: 300 }
                /*
                PropertyAnimation { id: sketchWingsReveal; targets: [sketchLeftWing, sketchRightWing]; property: "opacity"; to: 1; duration: 300 }
                PropertyAnimation { id: sketchWingsHide; targets: [sketchLeftWing, sketchRightWing]; property: "opacity"; to: 0; duration: 300 }
                PropertyAnimation { id: sketchButtonReveal; target: addButtonSketchRect; property: "opacity"; to: 1; duration: 300 }
                PropertyAnimation { id: sketchButtonHide; target: addButtonSketchRect; property: "opacity"; to: 0; duration: 300 }
                PropertyAnimation { id: sketchButtonMenuReveal; target: sketchButtonMenu; property: "opacity"; to: 1; duration: 150 }
                PropertyAnimation { id: sketchButtonMenuHide; target: sketchButtonMenu; property: "opacity"; to: 0; duration: 150 }
                */



            }

            Rectangle {                     // PRESENTATION BUTTON
                id: rectPres
                z: -1
                color: "#dbdea7"
                width: window.width/7.11
                height: (width/144)*137
                Image{
                    anchors.top: rectPres.top
                    anchors.topMargin: window.width/68.2
                    anchors.horizontalCenter: rectPres.horizontalCenter
                    width: window.width/11.5; height: window.width/11.63;         // CALCULATION: width: window.width/ (1024/ImageWidth)
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/mainmenu/presentation.png"
                }
                Text{
                    anchors.bottom: rectPres.bottom
                    anchors.horizontalCenter: rectPres.horizontalCenter
                    text: "presentation"
                    font.family: "Ubuntu"
                    font.pointSize: window.width/68
                    color: "black"
                }
                Image{
                    id: textPresBackgroundCircle
                    visible: false
                    x: window.width/11.37
                    y: window.width/204.8
                    //anchors.horizontalCenter: rectCollaboration.horizontalCenter
                    width: window.width/21.33; height: window.width/19.32    // CALCULATION: width: window.width/ (1024/ImageWidth)
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/mainmenu/noBackground.png"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        window.activeFunction = "presentation"
                        textPres.no=textPres.no+1;
                        if(textPres.no==0){
                            textPresBackgroundCircle.visible = false;
                            textPres.visible = false;
                        }
                        else{
                            textPresBackgroundCircle.visible = true;
                            textPres.visible = true;
                            activeCollaboration.visible = false;
                            activeSketch.visible = false;
                            activePresentation.visible = true;
                            activeWeb.visible = false;
                            activeMultimedia.visible = false;
                            activeConfig.visible = false;
                        }
                    }
                }
                Text{
                    id: textPres
                    visible: false
                    anchors.centerIn: textPresBackgroundCircle
                    property int no: 0
                    text: no
                    font.family: "Ubuntu"
                    font.pointSize: window.width/45
                    color: "black"
                }
                Rectangle{
                    id: activePresentation
                    visible: false;
                    anchors.bottom: parent.top
                    color: "#a8ad25"
                    width: window.width/7.11
                    height: (width/30)
                }
            }

            Rectangle {                     // WEB BUTTON
                id: rectWeb
                color: "#a8b3c7"
                width: window.width/7.11
                height: (width/144)*137
                Image{
                    anchors.top: rectWeb.top
                    anchors.topMargin: window.width/68.2
                    anchors.horizontalCenter: rectWeb.horizontalCenter
                    width: window.width/11.63; height: window.width/11.5;         // CALCULATION: width: window.width/ (1024/ImageWidth)
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/mainmenu/web.png"
                }
                Text{
                    anchors.bottom: rectWeb.bottom
                    anchors.horizontalCenter: rectWeb.horizontalCenter
                    text: "web"
                    font.family: "Ubuntu"
                    font.pointSize: window.width/68
                    color: "black"
                }
                Image{
                    id: textWebBackgroundCircle
                    visible: false
                    x: window.width/11.37
                    y: window.width/204.8
                    //anchors.horizontalCenter: rectCollaboration.horizontalCenter
                    width: window.width/21.33; height: window.width/19.32    // CALCULATION: width: window.width/ (1024/ImageWidth)
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/mainmenu/noBackground.png"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        window.activeFunction = "web"
                        textWeb.no=textWeb.no+1;
                        if(textWeb.no==0){
                            textWebBackgroundCircle.visible = false;
                            textWeb.visible = false;
                        }
                        else{
                            textWebBackgroundCircle.visible = true;
                            textWeb.visible = true;
                            activeCollaboration.visible = false;
                            activeSketch.visible = false;
                            activePresentation.visible = false;
                            activeWeb.visible = true;
                            activeMultimedia.visible = false;
                            activeConfig.visible = false;
                        }
                    }
                }
                Text{
                    id: textWeb
                    visible: false
                    anchors.centerIn: textWebBackgroundCircle
                    property int no: 0
                    text: no
                    font.family: "Ubuntu"
                    font.pointSize: window.width/45
                    color: "black"
                }
                Rectangle{
                    id: activeWeb
                    visible: false;
                    anchors.bottom: parent.top
                    color: "#254273"
                    width: window.width/7.11
                    height: (width/30)
                }
            }

            Rectangle {                     // MULTIMEDIA BUTTON
                id: rectMM
                color: "#a3d6d9"
                width: window.width/7.11
                height: (width/144)*137
                Image{
                    anchors.top: rectMM.top
                    anchors.topMargin: window.width/68.2
                    anchors.horizontalCenter: rectMM.horizontalCenter
                    width: window.width/11.77; height: window.width/12.19;         // CALCULATION: width: window.width/ (1024/ImageWidth)
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/mainmenu/multimedia.png"
                }
                Text{
                    anchors.bottom: rectMM.bottom
                    anchors.horizontalCenter: rectMM.horizontalCenter
                    text: "multimedia"
                    font.family: "Ubuntu"
                    font.pointSize: window.width/68
                    color: "black"
                }
                Image{
                    id: textMMBackgroundCircle
                    visible: false
                    x: window.width/11.37
                    y: window.width/204.8
                    //anchors.horizontalCenter: rectCollaboration.horizontalCenter
                    width: window.width/21.33; height: window.width/19.32    // CALCULATION: width: window.width/ (1024/ImageWidth)
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/mainmenu/noBackground.png"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        window.activeFunction = "multimedia"
                        textMM.no=textMM.no+1;
                        if(textMM.no==0){
                            textMMBackgroundCircle.visible = false;
                            textMM.visible = false;
                        }
                        else{
                            textMMBackgroundCircle.visible = true;
                            textMM.visible = true;
                            activeCollaboration.visible = false;
                            activeSketch.visible = false;
                            activePresentation.visible = false;
                            activeWeb.visible = false;
                            activeMultimedia.visible = true;
                            activeConfig.visible = false;
                        }
                    }
                }
                Text{
                    id: textMM
                    visible: false
                    anchors.centerIn: textMMBackgroundCircle
                    property int no: 0
                    text: no
                    font.family: "Ubuntu"
                    font.pointSize: window.width/45
                    color: "black"
                }
                Rectangle{
                    id: activeMultimedia
                    visible: false;
                    anchors.bottom: parent.top
                    color: "#17989d"
                    width: window.width/7.11
                    height: (width/30)
                }

            }

        }

        Rectangle {     // MENU UP/DOWN
            id: menuUpDown
            color: "#bc9e7e"
            width: window.width/23
            height: window.width*0.134

            Image{
                anchors.centerIn: parent
                width: window.width/26.94; height: window.width/15.05    // CALCULATION: width: window.width/ (1024/ImageWidth)
                fillMode: Image.PreserveAspectFit
                smooth: true
                source: "images/mainmenu/down.png"
            }


            PropertyAnimation { id: menuDownAnimation; target: bottomMenu; property: "y"; to: window.height; duration: 250 }
            PropertyAnimation { id: menuUpAnimation; target: bottomMenu; property: "y"; to: window.height - bottomMenu.height; duration: 250 }

            PropertyAnimation { id: upButtonAppearAnimation; target: upButtonRect; property: "opacity"; to: 0.8; duration: 250 }
            PropertyAnimation { id: upButtonDisappearAnimation; target: upButtonRect; property: "opacity"; to: 0; duration: 250 }

            MouseArea {
                anchors.fill: parent
                onClicked:{         //TODO: button acilimlari kapanacak + - ler gibi
                    menuDownAnimation.running = true
                    upButtonAppearAnimation.running = true
                }
            }

            Rectangle{
                id: upButtonRect
                opacity: 1
                anchors.bottom: parent.top
                anchors.left: parent.right
                anchors.leftMargin: window.width/180.0
                color: "#bc9e7e"
                z:1
                width: window.width/9
                height: window.width*0.04
                Image{
                    anchors.centerIn: parent
                    width: window.width/14.42; height: window.width/34.13    // CALCULATION: width: window.width/ (1024/ImageWidth)
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/mainmenu/up.png"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked:{
                        menuUpAnimation.running = true
                        upButtonDisappearAnimation.running = true
                    }
                }
            }

            /*Rectangle{
            id: activeUpDown
                visible: false;
                anchors.bottom: parent.top
                color: "#342323"
                width: window.width/17
                height: (width/13)
           }*/
        }

        Rectangle {     // BATTERY AND CONFIGURATION
            id: rectConfBatt
            color: "#dbdbdb";
            width: window.width/17
            height: (width/60)*137
            Column{
                anchors.centerIn: parent
                spacing: window.width/51.2     //TODO: STATIC
                Image{
                    //anchors.centerIn: parent
                    width: window.width/20.89; height: window.width/44.52    // CALCULATION: width: window.width/ (1024/ImageWidth)
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/mainmenu/battery.png"
                }
                Image{
                    //anchors.centerIn: parent
                    width: window.width/21.3; height: window.width/20.89    // CALCULATION: width: window.width/ (1024/ImageWidth)
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/mainmenu/config.png"
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    window.activeFunction = "config"
                    activeCollaboration.visible = false;
                    activeSketch.visible = false;
                    activePresentation.visible = false;
                    activeWeb.visible = false;
                    activeMultimedia.visible = false;
                    activeConfig.visible = true;
                }
            }
            Rectangle{
                id: activeConfig
                visible: false;
                anchors.bottom: parent.top
                color: "#342323"
                width: window.width/17
                height: (width/13)
            }
        }

        Rectangle {     // EXIT BUTTON
            id: rectExit
            color: "#f9bdbd";
            width: window.width/23
            height: window.width*0.134
            Image{
                anchors.centerIn: parent
                width: window.width/26.94; height: window.width/26.94    // CALCULATION: width: window.width/ (1024/ImageWidth)
                fillMode: Image.PreserveAspectFit
                smooth: true
                source: "images/mainmenu/exit2.png"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {

                    Qt.quit()
                }
            }
        }

    }

    Rectangle{          //PRES INTERFACE
        id: presInterface
        opacity: 0
        width: window.width
        height: window.width/15.0
        color: "#dbdea5"
        anchors.bottom: bottomMenu.top
        Row{
            id: presButtons
            spacing: window.width/100.0
            anchors.centerIn: parent;
            Rectangle{
                id: presShowPages
                width: window.width/6.7
                height: window.width/20.48
                color: "#f8f8ee" //TODO: onclicked: color=e4f87e
                Image{
                    anchors.centerIn: parent
                    width: window.width/7.11; height: window.width/31.03    // CALCULATION: width: window.width/ (1024/ImageWidth)
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/presImages/showPages.png"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {

                    }
                }
            }

            Rectangle{
                id: presFitHeight
                anchors.verticalCenter: parent
                width: window.width/20.48
                height: window.width/20.48
                color: "#f8f8ee"
                Image{
                    anchors.centerIn: parent
                    width: window.width/7.11; height: window.width/31.03    // CALCULATION: width: window.width/ (1024/ImageWidth)
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/presImages/fitPageToHeight.png"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {

                    }
                }
            }

            Rectangle{
                id: presFitWidth
                anchors.verticalCenter: parent
                width: window.width/20.48
                height: window.width/20.48
                color: "#f8f8ee"
                Image{
                    anchors.centerIn: parent
                    width: window.width/24.97; height: window.width/35.31    // CALCULATION: width: window.width/ (1024/ImageWidth)
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/presImages/fitPageToWidth.png"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {

                    }
                }
            }

            Rectangle{
                id: presZoomIn
                anchors.verticalCenter: parent
                width: window.width/20.48
                height: window.width/20.48
                color: "#f8f8ee"
                Image{
                    anchors.centerIn: parent
                    width: window.width/36.57; height: window.width/36.57    // CALCULATION: width: window.width/ (1024/ImageWidth)
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/presImages/zoomIn.png"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {

                    }
                }
            }

            Rectangle{
                id: presZoomOut
                anchors.verticalCenter: parent
                width: window.width/20.48
                height: window.width/20.48
                color: "#f8f8ee"
                Image{
                    anchors.centerIn: parent
                    width: window.width/36.57; height: window.width/36.57    // CALCULATION: width: window.width/ (1024/ImageWidth)
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "images/presImages/zoomOut.png"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {

                    }
                }
            }

        }
    }

    Rectangle{}
}

