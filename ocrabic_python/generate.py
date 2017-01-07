import csv
import random 

# load data from a csv file
def load_labeled_data(filename):
    labeled = {}
    f =  open(filename, 'r')
    csv_r = csv.reader(f)
    for row in csv_r:
        label = int(row[-1])
        data  = map(int, row[0:-1])
        if label in labeled :
            labeled[label].append(data)
        else :
            labeled[label] = [data]
    f.close()
    return labeled

# output vectors as csv
def save_as_csv(data, filename):
    f = open(filename, 'w')
    csv_w = csv.writer(f)
    for row in data:
        csv_w.writerow(row)
    f.close()

# count frequencies for data per label
def count_freq(data):
    f =  dict((i, len(data_dict[i])) for i in data_dict)
    return f, reduce(lambda x, y: x+y, f.values())

# generate a random dataset for examples that don't belong to a given label
def pick_complementary_set(label, freq, size):
    picks = []
    
    if label not in freq.keys() :
        raise Exception("Given label isn't among the loaded labels")
        
    while len(picks) < size :
        pick_label = label
        while(pick_label == label):
            pick_label = random.sample(freq.keys(), 1)[0]
        pick_example = random.randint(0, freq[pick_label] - 1)
        while (pick_label, pick_example) in picks :
            pick_example = random.randint(0, freq[pick_label] - 1)
        picks.append( (pick_label, pick_example) )
    
    return picks

# get picks from the dataset
def generate_complementary_set(picks, data):
    compset = []
    for pick in picks :
        compset.append(data[pick[0]][pick[1]])
    return compset

# given a label l, generate {[examples of l] + [examples of not(l)]}
def save_scv_dataset_for(label, filename, data, freqs):
    output = data[label]
    output_size = len(output)
    picks = pick_complementary_set(label, freqs, output_size)
    output.extend(generate_complementary_set(picks, data))
    save_as_csv(output, filename)

data_dict = load_labeled_data("data/data_sorted.csv")
freq_dict, total = count_freq(data_dict)

save_scv_dataset_for(0, "data/alef.csv", data_dict, freq_dict)