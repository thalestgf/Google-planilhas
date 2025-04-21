String seguirRedirecionamento(String url) {
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_MOVED_PERMANENTLY || httpCode == HTTP_CODE_FOUND) {
    String newUrl = http.getLocation();
    http.end();
    return seguirRedirecionamento(newUrl); // Segue o redirecionamento recursivamente
  } else {
    String response = http.getString();
    http.end();
    return response;
  }
}

bool escreverEmLista(String identificacao, int numDados, float dados[]) {
  bool flag_envio = 0;
  String url = googleScriptURL;
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<200> jsonDoc;
  jsonDoc["action"] = "escreverEmLista";
  jsonDoc["identificacao"] = identificacao;
  JsonArray jsonDados = jsonDoc.createNestedArray("dados");
  for (int i = 0; i < numDados; i++) {
    jsonDados.add(dados[i]);
  }

  String jsonString;
  serializeJson(jsonDoc, jsonString);

  int httpResponseCode = http.POST(jsonString);
  if (httpResponseCode == HTTP_CODE_MOVED_PERMANENTLY || httpResponseCode == HTTP_CODE_FOUND) {
    String newUrl = http.getLocation();
    http.end();
    seguirRedirecionamento(newUrl);
    Serial.println("Dados enviados");
    flag_envio = 1;
  } else if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.println("Erro ao enviar dados");
    flag_envio = 0;
  }
  http.end();
  return flag_envio;
}

bool escreverEmCelula(String identificacao, String celula, String dado) {
  bool flag_envio = 0;
  String url = googleScriptURL;
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<200> jsonDoc;
  jsonDoc["action"] = "escreverEmCelula";
  jsonDoc["identificacao"] = identificacao;
  jsonDoc["celula"] = celula;
  jsonDoc["dado"] = dado;

  String jsonString;
  serializeJson(jsonDoc, jsonString);

  int httpResponseCode = http.POST(jsonString);
  if (httpResponseCode == HTTP_CODE_MOVED_PERMANENTLY || httpResponseCode == HTTP_CODE_FOUND) {
    String newUrl = http.getLocation();
    http.end();
    seguirRedirecionamento(newUrl);
    Serial.println("Dados enviados");
    flag_envio = 1;
  } else if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.println("Erro ao enviar dados");
    flag_envio = 0;
  }
  http.end();
  return flag_envio;
}

String lerCelula(String identificacao, String celula) {
  String url = googleScriptURL;
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<200> jsonDoc;
  jsonDoc["action"] = "lerCelula";
  jsonDoc["identificacao"] = identificacao;
  jsonDoc["celula"] = celula;

  String jsonString;
  serializeJson(jsonDoc, jsonString);

  int httpResponseCode = http.POST(jsonString);
  if (httpResponseCode == HTTP_CODE_MOVED_PERMANENTLY || httpResponseCode == HTTP_CODE_FOUND) {
    String newUrl = http.getLocation();
    http.end();
    return seguirRedirecionamento(newUrl);
  } else if (httpResponseCode > 0) {
    String response = http.getString();
    http.end();
    return response;
  } else {
    http.end();
    return "Erro ao ler célula";
  }
}

String lerLinha(String identificacao, int linha) {
  String url = googleScriptURL;
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<200> jsonDoc;
  jsonDoc["action"] = "lerLinha";
  jsonDoc["identificacao"] = identificacao;
  jsonDoc["linha"] = linha;

  String jsonString;
  serializeJson(jsonDoc, jsonString);

  int httpResponseCode = http.POST(jsonString);
  if (httpResponseCode == HTTP_CODE_MOVED_PERMANENTLY || httpResponseCode == HTTP_CODE_FOUND) {
    String newUrl = http.getLocation();
    http.end();
    return seguirRedirecionamento(newUrl);
  } else if (httpResponseCode > 0) {
    String response = http.getString();
    http.end();
    return response;
  } else {
    http.end();
    return "Erro ao ler linha";
  }
}



void montarCabecalho(String _boardID, const String& colunaInicial, const std::vector<String>& cabecalhos) {
  // Verifica se a primeira célula já contém o primeiro cabeçalho esperado
  String celula = lerCelula(_boardID, colunaInicial + "1");

  if (celula != cabecalhos[0] && celula != "Erro ao ler célula") {
    Serial.print("Carregando cabeçalho...");
    Serial.println("");
    Serial.print("String recebida: ");
    Serial.println(celula);
    Serial.println("");

    // Percorre todas as colunas e escreve os cabeçalhos
    char coluna = colunaInicial[0];
    for (size_t i = 0; i < cabecalhos.size(); i++) {
      String celulaAlvo = String(coluna) + "1";

      while ( escreverEmCelula(_boardID, celulaAlvo, cabecalhos[i]) == 0);

      coluna++; // Avança para a próxima coluna

      // Se passar de 'Z', volta para 'A' (opcional, apenas se necessário)
      if (coluna > 'Z') coluna = 'A';
    }
  }
}
