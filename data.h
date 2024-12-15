#pragma once

#include <vector>

class data
{

public:

	data();
	~data();
	void setFeatureVector(std::vector<uint8_t>*);
	void appendToFeatureVector(uint8_t);
	void setLabel(uint8_t);
	void setEnumeratedLabel(int);

	int getFeatureVectorSize();
	uint8_t getLabel();
	uint8_t getEnumeratedLabel();
	std::vector<uint8_t>* getFeatureVector();
private:

	std::vector<uint8_t>* m_feature_vector; // no class at the end
	uint8_t m_label;
	int m_enum_label; //A-> 1, B->2




};
