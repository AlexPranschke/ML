#include "data.h"


data::data(){
	m_feature_vector = new std::vector<uint8_t>;
}

data::~data(){}

void data::setFeatureVector(std::vector<uint8_t>* vect){
	m_feature_vector = vect;
}
void data::appendToFeatureVector(uint8_t val) {

	 
	m_feature_vector->emplace_back(val);
}
void data::setLabel(uint8_t val) {

	m_label = val;
}
void data::setEnumeratedLabel(int) {}

int data::getFeatureVectorSize() {
	return m_feature_vector->size();
}
uint8_t data::getLabel() {
	return m_label;
}
uint8_t data::getEnumeratedLabel() {
	return m_enum_label;
}

std::vector<uint8_t>* data::getFeatureVector() {
	return m_feature_vector;
}