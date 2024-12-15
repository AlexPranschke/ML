#include "dataHander.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <memory>

dataHandler::dataHandler(){
   
    m_dataArray = std::make_unique<std::vector<std::shared_ptr<data>>>();
    m_testData = std::make_unique<std::vector<std::shared_ptr<data>>>();
    m_trainingData = std::make_unique<std::vector<std::shared_ptr<data>>>();
    m_validationData = std::make_unique<std::vector<std::shared_ptr<data>>>();
    // m_dataArray = new vector<data*>;
    
	
}
dataHandler::~dataHandler(){
	//free
}
void dataHandler::ReadFeatureVector(const std::string& path) {
    const size_t BUFFER_SIZE = 4096;

    uint32_t header[4];   // Nag³ówki do przechowywania wartoœci
    unsigned char bytes[4]; // Tablica do przechowywania 4 bajtów na raz

    std::ifstream f(path, std::ios::binary);
    if (!f.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }

    for (int i = 0; i < 4; i++) {
        f.read(reinterpret_cast<char*>(bytes), sizeof(bytes));  // Odczyt 4 bajtów
        if (f.gcount() != 4) {
            throw std::runtime_error("Error reading file header.");
        }

        // Debugowanie: wypisz odczytane bajty
        std::cout << "Read bytes: ";
        for (int j = 0; j < 4; ++j) {
            std::cout << "0x" << std::hex << (int)bytes[j] << " ";
        }
        std::cout << std::endl;

        // Konwersja na Little Endian
        header[i] = ConvertToLittleEndian(bytes);
        std::cout << "Converted header[" << i << "]: " << std::dec << header[i] << std::endl;
    }

    std::cout << "DONE GETTING FILE HEADER\n";
    std::cout << "First header value (number of items): " << header[1] << std::endl;

    int image_size = header[2] * header[3]; // Rozmiar pojedynczego obrazu
    m_dataArray->reserve(header[1]);        // Rezerwacja miejsca w wektorze

    std::vector<uint8_t> buffer(BUFFER_SIZE); // Bufor do odczytu
    std::cout << header[1];
    // Odczytuj dane dla ka¿dej klatki
    for (int i = 0; i < header[1]; i++) {
        std::unique_ptr<data> d = std::make_unique<data>();
        size_t total_bytes_read = 0;

        while (total_bytes_read < static_cast<size_t>(image_size)) {
            // Oblicz ile bajtów jeszcze potrzebujemy odczytaæ
            size_t bytes_to_read = std::min(static_cast<size_t>(image_size - total_bytes_read), BUFFER_SIZE);

            // Odczyt do bufora
            f.read(reinterpret_cast<char*>(buffer.data()), bytes_to_read); //f.read oczcekuje char * , a nie unsigned char * wiec konwersja
            size_t bytes_read = f.gcount();
            if (bytes_read == 0) {
                if (f.eof()) {
                    throw std::runtime_error("Unexpected end of file.");
                }
                else {
                    throw std::runtime_error("Error reading file.");
                }
            }

            // Dodaj dane z bufora do obiektu `d`
            for (size_t j = 0; j < bytes_read; j++) {
                d->appendToFeatureVector(buffer[j]);
            }

            total_bytes_read += bytes_read;
        }

        // Dodaj obiekt `d` do wektora
        m_dataArray->push_back(std::move(d));
    }

    std::cout << "Read and stored feature vectors: "  << std::endl;
}
//void dataHandler::ReadFeatureVector(const std::string & path) {
//    const size_t BUFFER_SIZE = 4096; // Rozmiar bufora dla odczytu
//
//    uint32_t header[4]; // magic number, number of images, rows, cols
//    unsigned char bytes[4];
//
//    FILE* f = fopen(path.c_str(), "rb");
//    if (!f) {
//        throw std::runtime_error("Could not open file: " + path);
//    }
//
//    
//
//    std::cout << "DONE GETTING FILE HEADER\n";
//
//    int image_size = header[2] * header[3]; // Rozmiar pojedynczego obrazu
//    m_dataArray->reserve(header[1]);       // Rezerwacja miejsca w wektorze
//
//    std::vector<uint8_t> buffer(BUFFER_SIZE); // Bufor do odczytu
//    size_t bytes_read = 0;
//
//    // Odczytuj dane dla ka¿dej klatki
//    for (int i = 0; i < header[1]; i++) {
//        auto d = std::make_unique<data>();
//        size_t total_bytes_read = 0;
//
//        while (total_bytes_read < static_cast<size_t>(image_size)) {
//            // Oblicz ile bajtów jeszcze potrzebujemy odczytaæ
//            size_t bytes_to_read = std::min(static_cast<size_t>(image_size - total_bytes_read), BUFFER_SIZE);
//
//            // Odczyt do bufora
//            bytes_read = fread(buffer.data(), sizeof(uint8_t), bytes_to_read, f);
//            if (bytes_read == 0) {
//                if (feof(f)) {
//                    fclose(f);
//                    throw std::runtime_error("Unexpected end of file.");
//                } else {
//                    fclose(f);
//                    throw std::runtime_error("Error reading file.");
//                }
//            }
//
//            // Dodaj dane z bufora do obiektu `d`
//            for (size_t j = 0; j < bytes_read; j++) {
//                d->appendToFeatureVector(buffer[j]);
//            }
//
//            total_bytes_read += bytes_read;
//        }
//
//        // Dodaj obiekt `d` do wektora
//        m_dataArray->push_back(std::move(d));
//    }
//
//    fclose(f);
//
//    std::cout << "Read and stored feature vectors: " << m_dataArray->size() << std::endl;
//}
void dataHandler::ReadFeatureLabels(std::string path) {
    
    const size_t BUFFER_SIZE = 4096; // Rozmiar bufora dla odczytu
    uint32_t header[2]; // magic number, number of items
    unsigned char bytes[4];

    std::ifstream f(path, std::ios::binary);
    if (!f.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }

    // Odczytaj nag³ówek pliku
    for (int i = 0; i < 2; i++) {
        f.read(reinterpret_cast<char*>(bytes), sizeof(bytes));
        if (f.gcount() != sizeof(bytes)) {
            throw std::runtime_error("Error reading file header.");
        }
        
        // Debugowanie: wypisz odczytane bajty
        std::cout << "Read bytes: ";
        for (int j = 0; j < 4; ++j) {
            std::cout << "0x" << std::hex << (int)bytes[j] << " ";
        }
        std::cout << std::endl;

        // Konwersja na Little Endian
        header[i] = ConvertToLittleEndian(bytes);
        std::cout << "Converted header[" << i << "]: " << std::dec << header[i] << std::endl;
    }
      
       
    std::cout << "DONE GETTING input FILE HEADER\n";

    
    m_dataArray->reserve(header[1]);        // Rezerwacja miejsca w wektorze

    std::vector<uint8_t> buffer(BUFFER_SIZE); // Bufor do odczytu

    // Odczytuj dane dla ka¿dej klatki
    size_t labels_read = 0;

    // Odczyt danych w pêtli
    while (labels_read < header[1]-1) {
        // Oblicz ile danych jeszcze pozosta³o do odczytania
        size_t remaining_labels = header[1] - labels_read;
        size_t bytes_to_read = std::min(remaining_labels, BUFFER_SIZE);

        // Odczyt do bufora
        f.read(reinterpret_cast<char*>(buffer.data()), bytes_to_read);
        size_t bytes_read = static_cast<size_t>(f.gcount());
        if (bytes_read == 0) {
            if (f.eof()) {
                throw std::runtime_error("Unexpected end of file.");
            }
            else {
                throw std::runtime_error("Error reading file.");
            }
        }

        // Przypisanie odczytanych danych do obiektów
        for (int  i = 0; i < bytes_read; i++) {
            if (labels_read + i < m_dataArray->size()) {
                m_dataArray->at(labels_read + i)->setLabel(buffer[i]);
                
            }
            else {
                std::cerr << "Out of range access at index: " << labels_read + i << std::endl;
                break;
                
            }
        }

        labels_read += bytes_read;
    }

      std::cout << "Read and assigned " << header[1] << " labels." << std::endl;
        

        // Dodaj obiekt `d` do wektora
}
void dataHandler::SplitData(){
    std::unordered_set<int> used_indexes;
    int train_size = m_dataArray->size() * m_TRAIN_SET_PERCENT;
    int test_size = m_dataArray->size() * m_TEST_SET_PERCENT;
    int valid_size = m_dataArray ->size() * m_VALIDATION_PERCENT;

    int count = 0;
    while (count < train_size) {
        long int rand_index = rand()% m_dataArray->size();
        if (used_indexes.find(rand_index) == used_indexes.end()) {
            m_trainingData->emplace_back(m_dataArray->at(rand_index));
            used_indexes.insert(rand_index);
            count++;
        }
    }
    count = 0;
    while (count < test_size) {
        double rand_index = rand() % m_dataArray->size();
        if (used_indexes.find(rand_index) == used_indexes.end()) {
            m_testData->emplace_back(m_dataArray->at(rand_index));
            used_indexes.insert(rand_index);
            count++;
        }
    }
    count = 0;
    while (count < valid_size) {
        double rand_index = rand() % m_dataArray->size();
        if (used_indexes.find(rand_index) == used_indexes.end()) {
            m_validationData->emplace_back(m_dataArray->at(rand_index));
            used_indexes.insert(rand_index);
            count++;
        }
    }
    std::cout << "Training Data " << m_trainingData->size() << std::endl;
    std::cout << "test Data " << m_testData->size() << std::endl;
    std::cout << "validation Data " << m_validationData->size() << std::endl;
    
    
    
}
void dataHandler::CountClasses(){
    int count = 0;
    for (int i = 0; i < m_dataArray->size(); i++) {
        {
            m_class_map[m_dataArray->at(i)->getLabel()] = count;
            m_dataArray->at(i)->setEnumeratedLabel(count);
            count++;

        }
    }
    m_num_classes = count;
    std::cout << "number of classes" << m_num_classes << std::endl;
}
uint32_t dataHandler::ConvertToLittleEndian(const unsigned char * bytes){
	return	(uint32_t(bytes[0]) << 24) |
			(uint32_t(bytes[1]) << 16) |
			(uint32_t(bytes[2]) << 8) |
			uint32_t(bytes[3]);
}
std::vector<std::shared_ptr<data>>& dataHandler::get_training_data(){
    return *m_trainingData;
}
std::vector<std::shared_ptr<data>>& dataHandler::get_test_data(){
    return *m_testData;
}
std::vector<std::shared_ptr<data>>& dataHandler::get_validation_data(){
    return *m_validationData;
}

