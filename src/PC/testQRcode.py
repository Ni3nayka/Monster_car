import cv2
import numpy as np
from pyzbar import pyzbar

video_cap = cv2.VideoCapture(2)#, cv2.CAP_DSHOW)

red = (0, 0, 255)
blue = (255, 0, 0)
qrcode_color = (255, 255, 0)
barcode_color = (0, 255, 0)

while True:
    # read and resize the frames
    success, frame = video_cap.read()
    if not success:
        print("YOU HAVE PROBLEM VITH CAMERA!!! (CODE DONT HAVE VIDEO)")
        break
    #frame = cv2.resize(frame, (480, 640))
    barcodes = pyzbar.decode(frame)
     # initialize the total number of QR codes and barcodes
    qr_code = 0
    code = 0

    for barcode in barcodes:
        pts = np.array([barcode.polygon], np.int32)
        pts = pts.reshape((-1,1,2))
        if barcode.type == "QRCODE":
            qr_code += 1
            cv2.polylines(frame, [pts], True, qrcode_color, 3)
        elif barcode.type == "CODE128":
            code += 1
            cv2.polylines(frame, [pts], True, barcode_color, 3)
    
        text = "{}".format(barcode.data.decode("utf-8"))
        cv2.putText(frame, text, (barcode.rect[0] + 10, barcode.rect[1] - 10), cv2.FONT_HERSHEY_SIMPLEX,
            0.5, red, 2)
    
    if len(barcodes) == 0:
        text = "No barcode detected"
        cv2.putText(frame, text, (20, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.75, blue, 2)
    else:
        text = "{} QR code(s) detected".format(qr_code)
        cv2.putText(frame, text, (20, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.75, blue, 2)
        text = "{} barcode(s) detected".format(code)
        cv2.putText(frame, text, (20, 43), cv2.FONT_HERSHEY_SIMPLEX, 0.75, blue, 2)

    cv2.imshow("frame", frame)
    # wait for 1 milliseconde and if the q 
    # key is pressed we break the loop
    if cv2.waitKey(1) == ord('q'):
        break
video_cap.release()
cv2.destroyAllWindows()