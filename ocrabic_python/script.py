import csv

# load csv data into a dict
def load_labeled_data(filename):
    labeled = {}
    f = open(filename, 'r')
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

# output files sorted by class/label
def save_data_sorted(data_dict):
    # sorting data
    with open('data/data_sorted.csv', 'w') as f:
        csv_w = csv.writer(f)
        for label in data_dict:
            for x in data_dict[label] :
                if label != 57 :
                    x.append(label)
                else :
                    x.append(36)
                csv_w.writerow(x)
        f.close()

# computing frequencies
def count_freq(data_dict):
    f =  dict((i, len(data_dict[i])) for i in data_dict)
    return f, reduce(lambda x, y: x+y, f.values())


# outputting each class to an independent file
def save_each_class(data_dict):
    for label in data_dict.keys() :
        with open('data/' + str(label) + '.csv', 'w') as f:
            csv_w = csv.writer(f)
            for row in data_dict[label]:
                csv_w.writerow(row)
            f.close()

labeled = load_labeled_data('data/data_sorted.csv')
print count_freq(labeled)