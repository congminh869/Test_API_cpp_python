static size_t _write_callback_(void contents, size_t size, size_t nmemb, void userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

struct EventThread : public MyThread {

	EventThread(queue_ev* queue_e) : MyThread{}, queue_event{queue_e}
	{
		std::string server_api = "http://192.168.6.120:5555";
		host = server_url + BASE_API_PATH;
		strres = "";

		/* Init curl*/
		struct curl_slist *headers = NULL;
		curl_slist_append(headers, "Content-Type: multipart/form-data");
		curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();
		if(!curl){
			cout << "Error init CURL\n";
			exit(-1);
		}

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		//set callback to recive response from server
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _write_callback_);
		//
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strres);
	}

	virtual ~EventThread() {
		curl_easy_cleanup(curl);
	}

	virtual int run() override
	{
		struct EVENT_DATA_SEND event;
		if(enable_event == 1)
		{
			if (!queue_event->pop(event, std::chrono::milliseconds(500))) {
				//cout << "cannot get event " << queue_event->size() << endl;
				return 0;
			}
			send_distancing(udid, event.filename, event.img, event.log);
		}
	}

	int send_distancing(std::string uid, std::string filename, cv::Mat img, std::string log)
	{
		/*Send image*/
		std::string url_img = host + "/image";

		std::vector<unsigned char> imgBuffer;
		imencode(".jpg", img, imgBuffer);

		//Set headers as null
		struct curl_httppost *formpost = NULL;
		struct curl_httppost *lastptr = NULL;
		//CURLFORM_COPYNAME = KEY, CURLFORM_COPYCONTENTS==VALUE
		curl_formadd(&formpost,
		        &lastptr,
		        CURLFORM_COPYNAME, "udid",
		        CURLFORM_COPYCONTENTS, uid.c_str(),
		        CURLFORM_END);

		curl_formadd(&formpost,
			&lastptr,
			CURLFORM_COPYNAME, "filename",
			CURLFORM_COPYCONTENTS, filename.c_str(),
			CURLFORM_END);
		//send buffer ben duoi, send file https://stackoverflow.com/questions/61190685/how-to-use-curl-formadd-upload-file-with-submit-fields-in-c
		curl_formadd(&formpost,
					 &lastptr,
					 CURLFORM_COPYNAME, "file",
					 CURLFORM_BUFFER, "file",
					 CURLFORM_BUFFERPTR, imgBuffer.data(),
					 CURLFORM_BUFFERLENGTH, imgBuffer.size(),
					 CURLFORM_END);

		curl_easy_setopt(curl, CURLOPT_URL, url_img.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

		CURLcode res = curl_easy_perform(curl);
		//khong gui dc, server ko nhan dc
		if (CURLE_OK != res) {
			cout << "curl_easy_perform error" << endl;
			cout << curl_easy_strerror(res) << endl;
			return 0;
		}
		std::cout << strres << std::endl;

		std::string str_img;
		rapidjson::Document doc;
		doc.Parse(strres.c_str());
		if(doc.HasMember("file_path"))
			str_img = doc["file_path"].GetString();
		else
			return 0;
		strres = "";

		/* Send log*/
		struct curl_httppost *formpost_t = NULL;
		struct curl_httppost *lastptr_t = NULL;
		curl_formadd(&formpost_t,
			        &lastptr_t,
			        CURLFORM_COPYNAME, "event_id",
			        CURLFORM_COPYCONTENTS, "6",
			        CURLFORM_END);

		curl_formadd(&formpost_t,
			        &lastptr_t,
			        CURLFORM_COPYNAME, "udid",
			        CURLFORM_COPYCONTENTS, uid.c_str(),
			        CURLFORM_END);

		curl_formadd(&formpost_t,
			&lastptr_t,
			CURLFORM_COPYNAME, "image_path",
			CURLFORM_COPYCONTENTS, str_img.c_str(),
			CURLFORM_END);

		curl_formadd(&formpost_t,
			&lastptr_t,
			CURLFORM_COPYNAME, "data",
			CURLFORM_COPYCONTENTS, log.c_str(),
			CURLFORM_END);

		std::string url_log = host + "/high_voltage_warning";

		curl_easy_setopt(curl, CURLOPT_URL, url_log.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost_t);

		res = curl_easy_perform(curl);
		if (CURLE_OK != res) {
			cout << "curl_easy_perform error" << endl;
			cout << curl_easy_strerror(res) << endl;
			return 0;
		}
		std::cout << strres << std::endl;
		strres = "";
		return 1;
	}

	virtual std::string name() override
	{
		return std::string{"EventThread"};
	}

	queue_ev * queue_event;

	CURL * curl;

	std::string host;

	std::string strres;
};

