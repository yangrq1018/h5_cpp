import h5py
import pandas as pd

filename = "data/data.h5"

with h5py.File(filename, "r") as f:
    print("Keys: %s" % f.keys())
    a_group_key = list(f.keys())[0]

    # Get the data
    # full dataset read
    data = list(f["000001.SZE"])

    print(len(data))

def hello_world():
    print("hello, world!")

# the pandas way
# without key=, pandas checks the first key available
# which happens to a named type object, not dataset
# filename = "data/data.h5"
# print(filename)
# df = pd.read_hdf("data/data.h5", key="points")
# print(df)

# read gzipped csv directly
# filename = "data/data.csv.gz"
# print(filename)
# df = pd.read_csv("data/data.csv.gz", compression="gzip")
# print(df)
