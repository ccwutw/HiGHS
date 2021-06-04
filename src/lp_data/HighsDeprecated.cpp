/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                       */
/*    This file is part of the HiGHS linear optimization suite           */
/*                                                                       */
/*    Written and engineered 2008-2021 at the University of Edinburgh    */
/*                                                                       */
/*    Available as open-source under the MIT License                     */
/*                                                                       */
/*    Authors: Julian Hall, Ivet Galabova, Qi Huangfu, Leona Gottwald    */
/*    and Michael Feldmeier                                              */
/*                                                                       */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**@file lp_data/HighsDeprecated.cpp
 * @brief
 */
#include "HConfig.h"
#include "Highs.h"

HighsStatus Highs::setHighsOptionValue(const std::string& option,
                                       const bool value) {
  deprecationMessage("setHighsOptionValue", "setOptionValue");
  return setOptionValue(option, value);
}

HighsStatus Highs::setHighsOptionValue(const std::string& option,
                                       const HighsInt value) {
  deprecationMessage("setHighsOptionValue", "setOptionValue");
  return setOptionValue(option, value);
}

HighsStatus Highs::setHighsOptionValue(const std::string& option,
                                       const double value) {
  deprecationMessage("setHighsOptionValue", "setOptionValue");
  return setOptionValue(option, value);
}

HighsStatus Highs::setHighsOptionValue(const std::string& option,
                                       const std::string value) {
  deprecationMessage("setHighsOptionValue", "setOptionValue");
  return setOptionValue(option, value);
}

HighsStatus Highs::setHighsOptionValue(const std::string& option,
                                       const char* value) {
  deprecationMessage("setHighsOptionValue", "setOptionValue");
  return setOptionValue(option, value);
}

HighsStatus Highs::readHighsOptions(const std::string filename) {
  deprecationMessage("readHighsOptions", "readOptions");
  return readOptions(filename);
}

HighsStatus Highs::passHighsOptions(const HighsOptions& options) {
  deprecationMessage("passHighsOptions", "passOptions");
  return passOptions(options);
}

HighsStatus Highs::getHighsOptionValue(const std::string& option, bool& value) {
  deprecationMessage("getHighsOptionValue", "getOptionValue");
  return getOptionValue(option, value);
}

HighsStatus Highs::getHighsOptionValue(const std::string& option,
                                       HighsInt& value) {
  deprecationMessage("getHighsOptionValue", "getOptionValue");
  return getOptionValue(option, value);
}

HighsStatus Highs::getHighsOptionValue(const std::string& option,
                                       double& value) {
  deprecationMessage("getHighsOptionValue", "getOptionValue");
  return getOptionValue(option, value);
}

HighsStatus Highs::getHighsOptionValue(const std::string& option,
                                       std::string& value) {
  deprecationMessage("getHighsOptionValue", "getOptionValue");
  return getOptionValue(option, value);
}

HighsStatus Highs::getHighsOptionType(const std::string& option,
                                      HighsOptionType& type) {
  deprecationMessage("getHighsOptionType", "getOptionType");
  return getOptionType(option, type);
}

HighsStatus Highs::resetHighsOptions() {
  deprecationMessage("resetHighsOptions", "resetOptions");
  return resetOptions();
}

HighsStatus Highs::writeHighsOptions(
    const std::string filename, const bool report_only_non_default_values) {
  deprecationMessage("writeHighsOptions", "writeOptions");
  return writeOptions(filename, report_only_non_default_values);
}

const HighsOptions& Highs::getHighsOptions() const {
  deprecationMessage("getHighsOptions", "getOptions");
  return getOptions();
}

HighsStatus Highs::setHighsLogfile(FILE* logfile) {
  deprecationMessage("setHighsLogfile", "None");
  options_.output_flag = false;
  return HighsStatus::kOk;
}

HighsStatus Highs::setHighsOutput(FILE* output) {
  deprecationMessage("setHighsOutput", "None");
  options_.output_flag = false;
  return HighsStatus::kOk;
}

const HighsInfo& Highs::getHighsInfo() const {
  deprecationMessage("getHighsInfo", "getInfo");
  return getInfo();
}

HighsStatus Highs::getHighsInfoValue(const std::string& info, HighsInt& value) {
  deprecationMessage("getHighsInfoValue", "getInfoValue");
  return getInfoValue(info, value);
}

HighsStatus Highs::getHighsInfoValue(const std::string& info,
                                     double& value) const {
  deprecationMessage("getHighsInfoValue", "getInfoValue");
  return getInfoValue(info, value);
}

HighsStatus Highs::writeHighsInfo(const std::string filename) {
  deprecationMessage("writeHighsInfo", "writeInfo");
  return writeInfo(filename);
}

double Highs::getHighsInfinity() {
  deprecationMessage("getHighsInfinity", "getInfinity");
  return getInfinity();
}

double Highs::getHighsRunTime() {
  deprecationMessage("getHighsRunTime", "getRunTime");
  return getRunTime();
}