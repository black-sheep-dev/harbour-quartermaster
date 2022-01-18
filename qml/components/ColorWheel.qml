import QtQuick 2.0

Item {
    property color rgb
    property real hue: 1
    property real saturation: 1

    function convertToHex(integer) {
        var str = Number(integer).toString(16)
        return str.length == 1 ? "0" + str : str
    }

    function hsvToRGB(h, s, v) {
        var r, g, b, i, f, p, q, t;
        if (arguments.length === 1) {
            s = h.s, v = h.v, h = h.h
        }
        i = Math.floor(h * 6)
        f = h * 6 - i
        p = v * (1 - s)
        q = v * (1 - f * s)
        t = v * (1 - (1 - f) * s)
        switch (i % 6) {
            case 0:
                r = v, g = t, b = p
                break
            case 1: r = q, g = v, b = p
                break
            case 2: r = p, g = v, b = t
                break
            case 3: r = p, g = q, b = v
                break
            case 4: r = t, g = p, b = v
                break
            case 5: r = v, g = p, b = q
                break

        }

        rgb = "#"
                + convertToHex(Math.round(r * 255))
                + convertToHex(Math.round(g * 255))
                + convertToHex(Math.round(b * 255))
    }

    id: root
    width: 200
    height: 200

    onHueChanged: hsvToRGB(hue, saturation, 1.0);
    onSaturationChanged: hsvToRGB(hue, saturation, 1.0);


    Rectangle {
        id: wheel
        anchors.fill: parent

        color: "transparent"

        ShaderEffect {
            anchors.fill: parent
            vertexShader: "
                            uniform highp mat4 qt_Matrix;
                            attribute highp vec4 qt_Vertex;
                            attribute highp vec2 qt_MultiTexCoord0;
                            varying highp vec2 coord;
                            void main() {
                                coord = qt_MultiTexCoord0 - vec2(0.5, 0.5);
                                gl_Position = qt_Matrix * qt_Vertex;
                        }"
            fragmentShader: "
                varying highp vec2 coord;
                vec3 hsv2rgb(in vec3 c){
                    vec4 k = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
                    vec3 p = abs(fract(c.xxx + k.xyz) * 6.0 - k.www);
                    return c.z * mix(k.xxx, clamp(p - k.xxx, 0.0, 1.0), c.y);
                }
                void main() {
                    const float PI = 3.14159265358979323846264;
                    float s = sqrt(coord.x * coord.x + coord.y * coord.y);
                    if( s > 0.5 ){
                        gl_FragColor = vec4(0, 0, 0, 0);
                        return;
                    }
                    float h = - atan( coord.y / coord.x );
                    s *= 2.0;
                    if( coord.x >= 0.0 ){
                        h += PI;
                    }
                    h = h / (2.0 * PI);
                    vec3 hsl = vec3(h, s, 1.0);
                    vec3 rgb = hsv2rgb(hsl);
                    gl_FragColor.rgb = rgb;
                    gl_FragColor.a = 1.0;
            }"
        }

        Item {
            id: picker
            x: parent.width/2 * (1 + root.saturation * Math.cos(2 * Math.PI * root.hue - Math.PI)) - r
            y: parent.width/2 * (1 + root.saturation * Math.sin(-2 * Math.PI * root.hue - Math.PI)) - r
            property int r : 8
            Rectangle {
                width: parent.r*2
                height: parent.r*2
                radius: parent.r
                border.color: "black"
                border.width: 2
                color: "transparent"
                Rectangle {
                    anchors.fill: parent
                    anchors.margins: 2
                    border.color: "white"
                    border.width: 2
                    radius: width/2
                    color: "transparent"
                }
            }
        }


        MouseArea {
            id: wheelArea
            function keepCursorInWheel(mouse, wheelArea, wheel) {
                root.state = 'editing'

                // cartesian to polar coords
                var ro = Math.sqrt(Math.pow(mouse.x-wheel.width/2,2)+Math.pow(mouse.y-wheel.height/2,2));
                var theta = Math.atan2(((mouse.y-wheel.height/2)*(-1)),((mouse.x-wheel.width/2)));

                // Wheel limit
                if(ro > wheel.width/2)
                    ro = wheel.width/2;

                // polar to cartesian coords
                var cursor = Qt.vector2d(0, 0);
                cursor.x = Math.max(-picker.r, Math.min(wheelArea.width, ro*Math.cos(theta)+wheel.width/2)-picker.r);
                cursor.y = Math.max(-picker.r, Math.min(wheelArea.height, wheel.height/2-ro*Math.sin(theta)-picker.r));

                hue = Math.ceil((Math.atan2(((cursor.y+picker.r-wheel.height/2)*(-1)),((cursor.x+picker.r-wheel.width/2)))/(Math.PI*2)+0.5)*100)/100
                saturation = Math.ceil(Math.sqrt(Math.pow(cursor.x+picker.r-width/2,2)+Math.pow(cursor.y+picker.r-height/2,2))/wheel.height*2*100)/100;
            }
            anchors.fill: parent
            onPositionChanged: keepCursorInWheel(mouse, wheelArea,  wheel)
            onReleased: keepCursorInWheel(mouse, wheelArea, wheel)
        }
    }
}
