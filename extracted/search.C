{

  std::string file = "dif_B5.dat";
  std::ifstream stream(file);
  std::string line;
  std::getline(stream, line);
  double max = 0;
  double current;
  while(getline(stream, line)){
    std::stringstream line_stream(line);
    std::string entry;
    std::vector<std::string> entries;
    char delim = ' ';

    while(getline(line_stream, entry, delim)){
      entries.push_back(entry);
    }

    current = std::stod(entries.at(11));
    if (current > max) max = current;
  }

  std::cout << "Max spotQ = " << max << std::endl;
}
    
