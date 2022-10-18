from flask import Flask, session, request, Response
import json
import base64
import numpy as np
from PIL import Image
import cv2
app = Flask(__name__)
def success_handle(output, status=200, mimetype='application/json'):
    return Response(output, status=status, mimetype=mimetype)

@app.route('/api/authentication_user', methods=['POST']) #send 4 image
def checkFaceImages():
    output = json.dumps({"success": True})
    id = request.form['id'] or None
    full_name = request.form['name'] or None
    birthday = request.form['dob'] or ''
    gender = request.form['gender'] or 0
    address = request.form['address'] or ''
    country = request.form['country'] or ''
    key =  request.form['key'] or None
    print('id: ', id)
    return success_handle(output)

@app.route('/api/receive', methods=['POST'])
def receive():
    output = json.dumps({"success": True})
    event = request.form
    # decode frame
    # shape_img = json.loads(request.form['shape_img'])
    # encoded_string = request.form['image']
    # decoded_string = base64.b64decode(encoded_string)
    # decoded_img = np.fromstring(decoded_string, dtype=np.uint8)
    # decoded_img = decoded_img.reshape(shape_img)
    # cv2.imshow("decoded", decoded_img)
    # cv2.waitKey(5)
    # print('decoded_img.shape : ', decoded_img.shape)
    # cv2.imwrite('decoded_img.jpg', decoded_img)
    
    id = request.form['id']
    type = request.form['type']
    timestr = request.form['time']
    data = request.form['data'] 
    # a = request.args

    print("id : ", id)
    print("type : ", type)
    print("timestr : ", timestr)
    print("data : ", data)

    #--python--
    file = request.form['image']
    # print("request.form : ", request.form)
    nparr = np.fromstring(file, np.uint8)
    ##################

    #---c++--------
    # file = request.files['image']
    # # print(file)
    # img = Image.open(file)
    # print(img)
    #####################
    # img.show()
    # if file and allowed_file(file.filename):
    #     print('**found file', file.filename)
    #     filename = secure_filename(file.filename)
    #     file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
    #     # for browser, add 'redirect' function on top of 'url_for'
    #     return url_for('uploaded_file',
    #                             filename=filename)
    print(nparr)
    return success_handle(output)

if __name__ =="__main__":  
    app.run(host="0.0.0.0", port=8000, debug=True)  