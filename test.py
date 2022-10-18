import requests
import datetime
import cv2
import base64
import json

# date_time = datetime.datetime.now()
# print(str(date_time))
# BASE = "http://127.0.0.1:5000/"

# response1 = requests.get(BASE + "helloworld")
# response2 = requests.post(BASE + "helloworld")
# response3 = requests.get(BASE + "helloworld/Minh/20")
# response = requests.patch(BASE + "video/2", {})
# print(response2.json())
# print(response1.json())
# print(response3.json())

# data = [{"likes": 78, "name": "minh", "views": 100000},
# 	  {"likes": 3, "name": "b", "views": 200},
# 	  {"likes": 4, "name": "c", "views": 500}]

# for i in range(len(data)):
# 	response= requests.put(BASE + "video/"+ str(i), data[i])
# 	print(response.json())

# input()
# response = requests.delete(BASE + "video/0")
# print(response)
# input()
# response = requests.get(BASE+"video/2")
# print(response.json())
# image = cv2.imread('a.png')
# print(image.shape)
# # encoded = base64.b64encode(open('a.png', "rb").read())
# encoded_string = base64.b64encode(image)
# img_encoded = cv2.imencode('.jpg', image)[1].tostring()
# for i in range(0,1):
# 	data = {'id': '192.168.6.66',
# 			 'type': "crowd",
# 			 'time': str(date_time), 
# 			 'data': '{"sample":true}',
# 			 'image': img_encoded}
# 	response = requests.post("http://0.0.0.0:8000/api/receive", data)
# 	print(response.json())

def send_image(id_key, type_key, time_key, data_key, img_encoded):
	BASE_API = "https://vms.demo.mqsolutions.vn/api/v1/event" #'http://0.0.0.0:8000/api/receive'
	data = {'ip': id_key,
			 'type': type_key,
			 'time': str(time_key), 
			 'data':data_key,
			 'image': img_encoded}
	response = requests.post(BASE_API, data)
	return response.json()

def send_image_json(frame):
    # _, img_encoded = cv2.imencode('.jpg', frame)#[1].tostring()
    headers = {'content-type': 'application/json'}

    img_encoded = base64.b64encode(frame)
    print('type(img_encoded) : ', type(img_encoded))
    print('type(img_encoded) : ', type(img_encoded.decode("utf-8")))
    # print(img_encoded.tostring())
    id_key = '192.168.6.66'
    type_key = 'crowd'
    time_key = str(datetime.datetime.now())
    print(' type(time_key):  ', type(time_key))
    data_key = {"sample": True, "json": True}
    img_encoded_tr = img_encoded.decode("utf-8")
    print('type(img_encoded_tr) : ', type(img_encoded_tr))
    files = '{\"sample\": true, \"json\": true}'#{'name': 'file1.jpg', 'data': img_encoded_tr, 'type': 'image'}

    BASE_API = 'https://vms.demo.mqsolutions.vn/api/v1/event-json'

    data = json.dumps({'ip': id_key,
             'type': type_key,
             'time': time_key, 
             'data':data_key,
             'files': [files]})
    data2= json.dumps({
                "ip": "192.168.6.66",
                "type": "crowd",
                "time": "2022-04-29 03:03:07.402607+00",
                "data": '{"sample": true, "json": true}',
                "files": [
                    {
                        "name": "file1.jpg",
                        "data": "/9j/6QAEAAD/2wBDABALChAYKDM9DAwOExo6PDcODRAYKDlFOA4RFh0zV1A+EhYlOERtZ00YIzdAUWhxXDFATldneXhlSFxfYnBkZ2P/2wBDARESGC9jY2NjEhUaQmNjY2MYGjhjY2NjYy9CY2NjY2NjY2NjY2NjY2NjY2NjY2NjY2NjY2NjY2NjY2NjY2NjY2P/xAAfAAABBQEBAQEBAQAAAAAAAAAAAQIDBAUGBwgJCgv/xAC1EAACAQMDAgQDBQUEBAAAAX0BAgMABBEFEiExQQYTUWEHInEUMoGRoQgjQrHBFVLR8CQzYnKCCQoWFxgZGiUmJygpKjQ1Njc4OTpDREVGR0hJSlNUVVZXWFlaY2RlZmdoaWpzdHV2d3h5eoOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3uLm6wsPExcbHyMnK0tPU1dbX2Nna4eLj5OXm5+jp6vHy8/T19vf4+fr/xAAfAQADAQEBAQEBAQEBAAAAAAAAAQIDBAUGBwgJCgv/xAC1EQACAQIEBAMEBwUEBAABAncAAQIDEQQFITEGEkFRB2FxEyIygQgUQpGhscEJIzNS8BVictEKFiQ04SXxFxgZGiYnKCkqNTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqCg4SFhoeIiYqSk5SVlpeYmZqio6Slpqeoqaqys7S1tre4ubrCw8TFxsfIycrS09TV1tfY2dri4+Tl5ufo6ery8/T19vf4+fr/wAARCACgAHADASIAAhEBAxEB/////9oADAMBAAIRAxEAPwDnhS4HrSZorsOcXIpN1BpMUgFzS5pCQO9V5LjP3fzqXIpIsZHqKN6/3hVMqT1NMIIqeYrlNDI7GiqCsR0NW4ZM9apMTRJRRRTRIGiiimIbRRS4pgFJilxTGf3pMaIZpSeg/GmKlOCUpOOlYM1SEBprU+3iLZ9P5U1higZGafG5HQ01jTAaExF+OTdT6qQOauKpPQVrEzkIDRU8doT9849h/WrKQqv3Rn61qkSZtBFBNFSMQ1DM1TNVec1nMqIoYetG0HvUAJFO3mszQvQyqo7f41UmYZ4pvmU1jmkAmaKKM0gJIW5rVgIHQc9zWOrYrTtZQR1P+H1rWmzORoAe9BNQm5jXqSx9B/jUL3jn7oCe4/xre/YzK1FFBqGUBqrcfT3zV1Iy33R+NV7uIjrWckUipmikFPC1mWNxRT9tGKQxlFPx60baBkeamiYgcH6j6VEVp8VVEllyJyeuKfTIxUlbIzY+K0kbr8g/vH+L6VajtI16jeeuT/hU7j/6woBFbWMrkTJjt9Kzb/FaUzgDrWRcMWPX2+nNZVDSBVC05atLZHH8qheBh1FYGo36Umaljjz1FEkRFKwyLrUiLntQLdj0zVqC3I60WC5SnXH+FNiNTakuCM/WqgamI0o/qKWoLdx3Pt9anGPWtUQzUJwPmIAqB7xB9wbjVV5Gb7zk+1Rk471rKXYyS7jp7h2+8x+gqi7e9SPMD92oXNYmiNjS28xf3jZIO3HpU8tqhPzCneHrVGjbfhWzujP+yB/F+NW/KJ/rXZGmuqOWrVd9DONkhI8sH/a9hTbm12n5RmtaGAJ/CM+pqlcyKT1/3sVM4roiYVWUVJHrU0Qz1p7vFjgjPeoTMF6/jWMjoTKerDkflWfUtxcMx56dqhrnZuh6uRTt59aYDRQhmmPeq88noaSSUn7px6+9QknvWjZmkSxqKLuPFNWQdqkn+ZeOvf3qogxLPUHiPygFerof4h7Gt2C7SQZtpGH95T1U+9czTo5WU5QkHsRWkKv8xlUpJ7HTSySAfNOx7VnbvemRalvH75mDdM9iPYU0yr/DVVJ9mZ06fdEg96hmkLcA8d/emtKaLZNx/WuaTOmMStcQ46fjVetO/AH1rMNZmoUoNJS0CJd1NZqQtTc1Qh4NShuOtVxTgxFNMVgNJS5pKllD0NTo1V1qeOgCYrVrT4vQfWqoPoauwSCNCf4v5GqSEzPv5MscdqpEVMeev1NMZaljRHS5pKWkAE0lFAFACg0GjFAFADgtJU8K5pjJQAtqgLDcRjpg960JdNx91mU9MfSs1VroUZygNwpDDox/jX/aNaRRLZUtdPx/rOfUVqtFGRho1K9MEf1rOmv44+jAn+6O1XIZ9w+fPqPbNW12IuUrzREAJtQwPXy+zZ/u5rFfI6gg9CPQ12cCkfeH0b2rL13RiQXtgOObhB3UdwPbvWUkWmc7RSZHY5pakoSlFJRQA6ikFKDTAnt3A6/hTyoPSoEqdaBMksLbdIoxnJA/XvXXXMOw/d+X7u0/w/WsPwvDuuFzyBmQj6DvXV6nAB/qx7n610GM2cbr+mJCQYVIQnDgfwNjtn1qVbg7FeMAkfJKn94fWtLUoDLEw6sv7xMf3kNYmlSkgqSMHn6fSoHE6C2u1IGCGQ/cb+6ff6VZjlHYjP8A6F9RXKWd41uxB5jPE0f07p9K6OAowzDIGXG8N6j6UMo5/wAQaWkZ3W6hVPEsY/5ZyH/4oVkVr67qnmnCH5R3/vVkVizRH//Z//8=",
                        "type": "image"
                    }
                ]
            })
    print(type(data2))

    response = requests.post(BASE_API, data2, headers=headers)
    # r = requests.post(url, data, headers=headers)
    print(response.json())
    return response.json()

def send_image_test2(frame):
    # _, img_encoded = cv2.imencode('.jpg', frame)#[1].tostring()
    print(type(frame))
    headers = {'content-type': 'application/json'}
    img_encoded = cv2.imencode('.jpg', frame)[1].tobytes()
    print('type(img_encoded) : ', type(img_encoded))
    img_encoded_base64 = base64.b64encode(img_encoded).decode("utf-8")
    print('type(img_encoded_base64) : ', type(img_encoded_base64))

    id_key = '192.168.6.66'
    type_key = 'crowd'
    time_key = str(datetime.datetime.now())
    data_key = '{"sample": true, "json": true}'
    # img_encoded_tr = img_encoded.decode("utf-8")
    files = {'name': 'file1.jpg', 'data': img_encoded_base64, 'type': 'image'}

    BASE_API = 'https://vms.demo.mqsolutions.vn/api/v1/event-json'

    data = json.dumps({'ip': id_key,
             'type': type_key,
             'time': time_key, 
             'data':data_key,
             'files': [files]})

    response = requests.post(BASE_API, data, headers=headers)
    # r = requests.post(url, data, headers=headers)
    print(response.json())
    return response.json()



if __name__ == '__main__':
	image = cv2.imread('a.png')
	
	id_key = '192.168.6.66'
	type_key = 'crowd'
	time_key = datetime.datetime.now()
	data_key = '{"sample":true}'
	for i in range(0,10):
		# response = send_image(id_key, type_key, time_key, data_key, img_encoded)
		send_image_test2(image)
		print('--')
		# print(response)