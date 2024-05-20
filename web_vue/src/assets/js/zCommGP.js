
function HexStringToBytes(hexString) {
    var bytes = [];
    if(hexString.length < 6){
        hexString = hexString.padStart(6,"0");
    }
    for (var i = 0; i < hexString.length; i += 2) {
        bytes.push(parseInt(hexString.substr(i, 2), 16));
    }
    console.log(hexString);
    return bytes;
}

// 将字节数据进行 Base64 编码
function ToBase64(bytes) {
    var binary = '';
    for (var i = 0; i < bytes.length; i++) {
        binary += String.fromCharCode(bytes[i]);
    }
    return btoa(binary);
}

export const GP_Code = {

    SetLEDColor:0x10,
    ResLEDColor:0x11,
};


const octbase64flag = "enB3B-Base64";

// [type][des addr][[src data][count][data][check sum]]
export function GP_PackData(code,indata){
    var data = [];
    var res = octbase64flag;
    var bytesdata = [];
    var checksum = 0;
    if(typeof(code) != "number"){
        return NaN;
    }
    if(typeof(indata) == "string"){
        bytesdata = HexStringToBytes(indata);
    }
    else{
        bytesdata = indata;
    }
    data.push(0x80);
    data.push(0x00);
    data.push(0x00);
    checksum += 0x00;
    data.push(0x01);
    checksum += 0x01;
    data.push(bytesdata.length);
    checksum += bytesdata.length;
    data.push(code);
    checksum+=code;
    bytesdata.forEach(function(val){
        data.push(val);
        checksum+=val;
    });
    data.push(checksum%256);

    console.log("data:",data);
    res += ToBase64(data);
    console.log("res:",res);

    return res;
}
