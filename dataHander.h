#pragma once

#include <vector>
#include <fstream>
#include "data.h"
#include <string>
#include <map>
#include <unordered_set>

class dataHandler
{

public:
	dataHandler();
	~dataHandler();

	void ReadFeatureVector(const std::string & path);
	void ReadFeatureLabels(std::string path);
	void SplitData();
	void CountClasses();
	uint32_t ConvertToLittleEndian(const unsigned char *bytes);
	std::vector<std::shared_ptr<data>>& get_training_data();
	std::vector<std::shared_ptr<data>>& get_test_data();
	std::vector<std::shared_ptr<data>>& get_validation_data();
private:
	std::unique_ptr<std::vector<std::shared_ptr<data>>> m_dataArray; // wszystkie dane
	std::unique_ptr<std::vector<std::shared_ptr<data>>> m_trainingData;
	std::unique_ptr<std::vector<std::shared_ptr<data>>> m_testData;
	std::unique_ptr<std::vector<std::shared_ptr<data>>> m_validationData;
	//std::vector<data*>* m_validationData;

	int m_num_classes;
	int m_feature_vector_size;
	std::map<uint8_t, int > m_class_map;
	const double m_TRAIN_SET_PERCENT = 0.75;
	const double m_TEST_SET_PERCENT = 0.20;
	const double m_VALIDATION_PERCENT = 0.05;




};
