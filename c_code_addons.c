/*"ESP32 Camera Surveillance Car" exaple addons*/
void startCameraServer(){
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    httpd_uri_t ajax_uri = {
      .uri       = "/fctrl",
      .method    = HTTP_GET,
      .handler    = mainfuncajax_handler,
      .user_ctx  = NULL
    };
}

static esp_err_t mainfuncajax_handler(httpd_req_t *req){
  //Serial.println("mainfuncajax");
  
 int lt=0;
 int rt=0;


 char*  buf;
    size_t buf_len;
    char variable[32] = {0,};
    char value[32] = {0,};

    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = (char*)malloc(buf_len);
        if(!buf){
            return httpd_resp_send_500(req);
             
        }
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            if (httpd_query_key_value(buf, "l", variable, sizeof(variable)) == ESP_OK &&
                httpd_query_key_value(buf, "r", value, sizeof(value)) == ESP_OK) {
            } else {
                free(buf);
                return httpd_resp_send_404(req);
                 
            }
        } else {
            free(buf);
            
            return httpd_resp_send_404(req);
        }
        free(buf);
    } else {
        httpd_resp_send_404(req);
        return httpd_resp_send_404(req);
    }

    lt = atoi(variable);
    rt = atoi(value);

  if (lt>100) {
    lt=100;
  }

  if (lt<-100) {
    lt=-100;
  }

  if (rt>100) {
    rt=100;
  }

  if (rt<-100) {
    rt=-100;
  }
  
  rd=rt;
  ld=lt;
  
  httpd_resp_set_type(req, "text/html");
  String p=String("");
  p+="r:"+String(rd);
  p+=String("l:");
  p+=String(ld);
  return httpd_resp_send(req, p.c_str(), p.length());
}


void analogWrite(int p,int v ){

  if (v==0) {
     return; 
  }
  v=36+round( 64*((float)v/100));

  if (p==gpleftforward) if (v<counter) digitalWrite(gpleftforward,LOW); else digitalWrite(gpleftforward,HIGH);
  if (p==gpleftback) if (v<counter) digitalWrite(gpleftback,LOW); else digitalWrite(gpleftback,HIGH);
  if (p==gprightforward) if (v<counter) digitalWrite(gprightforward,LOW); else digitalWrite(gprightforward,HIGH);
  if (p==gprightback) if (v<counter) digitalWrite(gprightback,LOW); else digitalWrite(gprightback,HIGH);
  
}

void moveer(){

  if (ld==0) {
    digitalWrite(gpleftforward,LOW);
    digitalWrite(gpleftback,LOW);
  }else{
    if (ld>0) {
      digitalWrite(gpleftback,LOW);
      analogWrite(gpleftforward,abs(ld));
    }else{
      digitalWrite(gpleftforward,LOW);
      analogWrite(gpleftback,abs(ld));
    }
  }
   
  if (rd==0) {
    digitalWrite(gprightforward,LOW);
    digitalWrite(gprightback,LOW);
  }else{
    if (rd>0) {
      digitalWrite(gprightback,LOW);
      analogWrite(gprightforward,abs(rd));
    }else{
      digitalWrite(gprightforward,LOW);
      analogWrite(gprightback,abs(rd));
    }
  }
 
}

void loop() 
{

  moveer();
  
  if (counter==100){
    counter=0;
  }
  counter++;
}
