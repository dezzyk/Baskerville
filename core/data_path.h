//
// Created by Feed on 5/31/2020.
//

#pragma once
#include <string>
#ifdef BASKERVILLE_IN_DEV
static const std::string global_data_path = "../data/";
#else
static const std::string global_data_path = "data/";
#endif
