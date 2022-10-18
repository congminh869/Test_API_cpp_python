#include <iostream>
#include <curl/curl.h>
#include <opencv2/opencv.hpp>
/*
https://curl.se/libcurl/c/curl_formadd.html
https://curl.se/libcurl/c/http-post.html
*/
std::string strres;

std::string return_current_time_and_date()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
    const std::chrono::duration<double> tse = t.time_since_epoch();
    std::chrono::seconds::rep milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(tse).count() % 100000000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S.")<< milliseconds; // %Y-%m-%d %H:%M:%S
    return ss.str();
}

#define DEBUG_PRINT
#ifdef DEBUG_PRINT
#define DEBUG_Log()                                                        \
{                                                                                    \
    std::cout << "[" << return_current_time_and_date() << "]Debug log [" << __FILE__ << "][" << __LINE__ << "]\n";        \
}
#else
#define DEBUG_Log(){}                                                       
#endif

int send_distancing(std::string uid, std::string type, std::string time, std::string data, cv::Mat img, CURL * curl, std::string host)
{
	//udid, type, time, data
	/*Send image*/
	std::string url_img = host;

	std::vector<unsigned char> imgBuffer;
	cv::imencode(".jpg", img, imgBuffer);

	//Set headers as null
	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	curl_formadd(&formpost,
	        &lastptr,
	        CURLFORM_COPYNAME, "ip",
	        CURLFORM_COPYCONTENTS, uid.c_str(),
	        CURLFORM_END);
	curl_formadd(&formpost,
	        &lastptr,
	        CURLFORM_COPYNAME, "type",
	        CURLFORM_COPYCONTENTS, type.c_str(),
	        CURLFORM_END);
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "time",
		CURLFORM_COPYCONTENTS, time.c_str(),
		CURLFORM_END);
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "data",
		CURLFORM_COPYCONTENTS, data.c_str(),
		CURLFORM_END);
	curl_formadd(&formpost,
				 &lastptr,
				 CURLFORM_COPYNAME, "image",
				 CURLFORM_BUFFER, "image",
				 CURLFORM_BUFFERPTR, imgBuffer.data(),
				 CURLFORM_BUFFERLENGTH, imgBuffer.size(),
				 CURLFORM_CONTENTTYPE, "image/jpg",
				 CURLFORM_END);

	curl_easy_setopt(curl, CURLOPT_URL, url_img.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	CURLcode res;
	res = curl_easy_perform(curl);
	if (CURLE_OK != res) {
		std::cout << "curl_easy_perform error" << std::endl;
		std::cout << curl_easy_strerror(res) << std::endl;
		return 0;
	}
	std::cout << strres << std::endl;
	strres = "";
	return 1;
}

int send_distancing_2(std::string uid, std::string type, std::string time, std::string data, cv::Mat img, CURL * curl, std::string host, std::string strres)
{
	DEBUG_Log();
	//udid, type, time, data
	/*Send image*/
	std::string url_img = host;

	// std::vector<unsigned char> imgBuffer;
	// cv::imencode(".jpg", img, imgBuffer);
	// std::cout<<"imgBuffer.data() : "<<img.rows<<std::endl;
	// std::cout<<"imgBuffer.data() : "<<img.cols<<std::endl;
	// std::cout<<"imgBuffer.data() : "<<imgBuffer.data()<<std::endl;
	// std::cout<<"imgBuffer.size() : "<<imgBuffer.size() <<std::endl;

	std::vector<uchar> vec_Img;

	std::vector<int> vecCompression_params;

	vecCompression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
	vecCompression_params.push_back(90);
	cv::imencode(".jpg", img, vec_Img, vecCompression_params);

	//Set headers as null
	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	DEBUG_Log();
	curl_formadd(&formpost,
	        &lastptr,
	        CURLFORM_COPYNAME, "id",
	        CURLFORM_COPYCONTENTS, uid.c_str(),
	        CURLFORM_END);
	DEBUG_Log();
	curl_formadd(&formpost,
	        &lastptr,
	        CURLFORM_COPYNAME, "type",
	        CURLFORM_COPYCONTENTS, type.c_str(),
	        CURLFORM_END);
	DEBUG_Log();
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "time",
		CURLFORM_COPYCONTENTS, time.c_str(),
		CURLFORM_END);
	DEBUG_Log();
	/*curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "data",
		CURLFORM_COPYCONTENTS, data.c_str(),
		CURLFORM_END);*/
	DEBUG_Log();
	curl_formadd(&formpost,
				 &lastptr,
				 CURLFORM_COPYNAME, "data",
				 CURLFORM_BUFFER, "image.jpg",
				 CURLFORM_BUFFERPTR, vec_Img.data(),
				 CURLFORM_BUFFERLENGTH, vec_Img.size(),
				 CURLFORM_CONTENTTYPE, "image/jpg",
				 CURLFORM_END);

	curl_formadd(&formpost,
					 &lastptr,
					 CURLFORM_COPYNAME, "file",
					 CURLFORM_BUFFER, "file",
					 CURLFORM_BUFFERPTR, vec_Img.data(),
					 CURLFORM_BUFFERLENGTH, vec_Img.size(),
					 CURLFORM_END);

	DEBUG_Log();

	// curl_formadd(&formpost,
	// 	&lastptr,
	// 	CURLFORM_COPYNAME, "data2",
	// 	CURLFORM_COPYCONTENTS, data.c_str(),
	// 	CURLFORM_END);

	curl_easy_setopt(curl, CURLOPT_URL, url_img.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	DEBUG_Log();
	CURLcode res;
	DEBUG_Log();
	res = curl_easy_perform(curl);
	DEBUG_Log();
	if (CURLE_OK != res) {
		DEBUG_Log();
		std::cout << "curl_easy_perform error" << std::endl;
		std::cout << curl_easy_strerror(res) << std::endl;
		return 0;
	}
	DEBUG_Log();
	std::cout <<"strres : " <<strres << std::endl;
	strres = "";
	DEBUG_Log();
	return 1;
}

static size_t _write_callback_(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


int main(int argc, char const *argv[])
{
	/*--init--*/
	DEBUG_Log();
	CURL * curl;
	std::string host;
	std::string server_api = "https://vms.demo.mqsolutions.vn/";
	std::string BASE_API_PATH = "api/v1/event";
	host = server_api + BASE_API_PATH;
	strres = "";
	cv::Mat img = cv::imread("/media/minhssd/New Volume/work/Documents/hoctap/REST_API_Tutorial/decoded_img.jpg");

	/* Init curl*/
	struct curl_slist *headers = NULL;
	curl_slist_append(headers, "Content-Type: multipart/form-data");
	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);
	/* get a curl handle */
	curl = curl_easy_init();
	if(!curl){
		std::cout << "Error init CURL\n";
		exit(-1);
	}
	DEBUG_Log();
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _write_callback_);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strres);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

	/*data*/
	//send_distancing(std::string uid, std::string filename, cv::Mat img, std::string log)
	std::string udid = "192.168.6.66";
	std::string type = "crowd";
	std::string time = return_current_time_and_date();
	std::string data = "{\"sample\":true}";
	DEBUG_Log();
	/*sending*/
	for(int i; i<50 ; i++){
		send_distancing(udid, type, time, data, img, curl, host);
		std::cout<<"===============================\n";
		strres="";
	}

	return 0;
}