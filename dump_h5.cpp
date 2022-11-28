/*
 * Demonstrate how to write a list of C++ structs to HDF5 file
 */
#include <time.h>
#include <vector>
#include "highfive/H5File.hpp"
#include <fstream>
#include <cstring>
#include <iostream>


typedef struct {
    char FieldType;
    char SecurityID[31];
    char TradingDay[8];
    int ExchangeTime;
    int Index;
    double Price;
    int Volume;
    char chOrderKind;
    char chFunctionCode;
    int Turnover;
    int BSFlag;
    int AskOrder;
    int BidOrder;
    /* struct timeval tv; */
} OrderFlow;


// Declare H5 data types
HighFive::CompoundType create_compound_OrderFlow() {
    return {
        {"FieldType", HighFive::AtomicType<char>{}},
        {"SecurityID", HighFive::AtomicType<char[31]>{}},
        {"TradingDay", HighFive::AtomicType<char[8]>{}},
        {"ExchangeTime", HighFive::AtomicType<int>{}},
        {"Index", HighFive::AtomicType<int>{}},
        {"Price", HighFive::AtomicType<double>{}},
        {"Volume", HighFive::AtomicType<int>{}},
        {"chOrderKind", HighFive::AtomicType<char>{}},
        {"chFunctionCode", HighFive::AtomicType<char>{}},
        {"Turnover", HighFive::AtomicType<int>{}},
        {"BSFlag", HighFive::AtomicType<int>{}},
        {"AskOrder", HighFive::AtomicType<int>{}},
        {"BidOrder", HighFive::AtomicType<int>{}},
    };
}
HIGHFIVE_REGISTER_TYPE(OrderFlow, create_compound_OrderFlow)


OrderFlow gen_random_data(const std::string secuID, int i) {
    int random_num = (i % 100);
    OrderFlow data = {
        .FieldType = 'x',
        .ExchangeTime = 93000 * random_num,
        .Index = 1 * random_num,
        .Price = 300.29 * random_num,
        .Volume = 100 * random_num,
        .chOrderKind = 'b',
        .chFunctionCode = 'f',
        .Turnover = 10000 * random_num,
        .BSFlag = 1 * (i % 100),
        .AskOrder = 33294 * random_num,
        .BidOrder = 32092 * random_num,
    };
    strcpy(data.SecurityID, secuID.c_str());
    strncpy(data.TradingDay, "20210528", 8);
    return data;
}

void write_to_csv(std::ofstream& out, const OrderFlow& data) {
    out << data.FieldType << ','
        << std::string(data.SecurityID, 31) << ','
        << std::string(data.TradingDay, 8) << ','
        << data.ExchangeTime << ','
        << data.Index << ','
        << data.Price << ','
        << data.Volume << ','
        << data.chOrderKind << ','
        << data.chFunctionCode << ','
        << data.Turnover << ','
        << data.BSFlag << ','
        << data.AskOrder << ','
        << data.BidOrder << std::endl;
    return;
}


const std::string secuIDs[] = {
    "000001.SZE",
    "600519.SSE"
};

int data_io(const std::string dt, const int DATASET_SIZE = 80000) {
    HighFive::File file("data/" + dt + ".h5", HighFive::File::Truncate);
    std::ofstream fs("data/" +dt + ".csv");

    auto t1 = create_compound_OrderFlow();
    t1.commit(file, "OrderFlow");

    for (const auto& secuID: secuIDs) {
        std::cout << secuID << std::endl;
        const std::string DATASET_NAME(secuID);
        std::vector<OrderFlow> pts = {};
        for (int i=0; i<DATASET_SIZE; i++) {
            auto data = gen_random_data(secuID, i);
            pts.push_back(data);
            // write to csv
            write_to_csv(fs, data);
        }

        // Compression, max level
        // Chunking: 1D
        HighFive::DataSetCreateProps dsprops;
        dsprops.add(HighFive::Chunking(std::vector<hsize_t>{100,}));
        dsprops.add(HighFive::Deflate(9));
        auto dataset = file.createDataSet(DATASET_NAME, pts, dsprops);
    }
    
    // the group attribute is stored in header, so data can't be too large
    /* auto g1 = file.createGroup("group1"); */
    /* g1.createAttribute(DATASET_NAME, pts); */
    return 0;
}

int main() {
    return data_io("20210528");
}
