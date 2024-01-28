#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <unordered_map>
#include <string>

using namespace std;

class CollaborativeFiltering {
    public:
        vector<vector<float> > data;
        // vector<vector<float> > similarities_total;

        bool isUB = true;

        void loadData(const string& filepath, bool user_based){
            ifstream file(filepath);
            string line;
            map<int, int> user_count, item_count;

            isUB = user_based;

            // Read the header line
            getline(file, line);

            while(getline(file, line)){
                int user_id, item_id;
                float rating;

                // Split the line by commas

                stringstream ss(line);
                getline(ss, line, ',');
                user_id = stoi(line);
                getline(ss, line, ',');
                item_id = stoi(line);
                getline(ss, line, ',');
                rating = stof(line);

                // Count The number of rating for user and item
                user_count[user_id]++;
                item_count[item_id]++;

                if (isUB == true){
                    if (data.size() <= user_id){
                        data.resize(user_id + 1);
                    }
                    if (data[user_id].size() <= item_id) {
                        data[user_id].resize(item_id + 1);
                    }

                    // Store the rating in the 2D vector
                    data[user_id][item_id] = rating;
                }
                else {
                    if (data.size() <= item_id){
                        data.resize(item_id + 1);
                    }
                    if (data[item_id].size() <= user_id) {
                        data[item_id].resize(user_id + 1);
                    }

                    // Store the rating in the 2D vector
                    data[item_id][user_id] = rating;
                }
            }

            // Store count in vector to find top user Count and item count
            vector<pair<int, int> > user_counter, item_counter;
            for(auto const& count: user_count){
                user_counter.push_back(make_pair(count.first, count.second));
            }
            for(auto const& count: item_count){
                item_counter.push_back(make_pair(count.first, count.second));
            }

            // sort the user counts and item counts
            sort(user_counter.begin(), user_counter.end(), sortingfunc);
            sort(item_counter.begin(), item_counter.end(), sortingfunc);

            // print the counts
            for (int i =0; i < 10; i++){
                cout << "User " << user_counter[i].first << " Had rated " << user_counter[i].second << " items" << endl;
                // cout << "Item " << item_counter[i].first << " Had " << item_counter[i].second << " ratings" << endl;
            }
            for (int i =0; i < 10; i++){
                // cout << "User " << user_counter[i].first << " Had rated " << user_counter[i].second << " items" << endl;
                cout << "Item " << item_counter[i].first << " Had " << item_counter[i].second << " ratings" << endl;
            }

            // compute the similarity
            // similarities_total = computeSimilarity(data);
        }

        // Calculate the similarity between the target user and all other users
        vector<pair<int, float> > computeSimilarity(int user_id) const {
            vector<pair<int, float> > similarities;
            
            for(int other_user_id = 0; other_user_id < data.size(); other_user_id++){

                if (other_user_id == user_id){
                    similarities.push_back(make_pair(other_user_id, 1));
                    continue;
                }
                float similarity = 0.0;
                float nom = 0.0;
                float denom1 = 0.0;
                float denom2 = 0.0;
                for (int item = 0; item < data[other_user_id].size(); item++) {
                    if (item < data[user_id].size()){
                        denom1 += data[user_id][item] * data[user_id][item];
                        nom += data[user_id][item] * data[other_user_id][item];
                    }
                    denom2 += data[other_user_id][item] * data[other_user_id][item];
                }

                float denom = std::sqrt(denom1) * std::sqrt(denom2);
                if (denom != 0.0){
                    similarity = nom / denom;
                }

                similarities.push_back(make_pair(other_user_id, similarity));
            }
            cout << user_id << endl;

            return similarities;
        }

        float scaledfunc(float value, float min, float max, vector<pair<int, float> >& arr) const {
            float a_max = arr[1].second;
            float a_min = arr[arr.size()-1].second;

            float x_div = (value - a_min) / (a_max - a_min);
            float scaled = x_div * (max - min) + min;
            
            return scaled;
        }

        static bool sortingfunc(const std::pair<int, float>& a, const std::pair<int, float>& b) {
            return a.second > b.second;
        }

        float predictRating(int user_id, int item_id) const {
            float predectedRating = 0.0;
            // if (user_id < 0 || user_id >= data.size()) {
            //     throw std::invalid_argument("Invalid user ID");
            // }
            // if (item_id < 0 || item_id >= data[user_id].size()) {
            //     throw std::invalid_argument("Invalid item ID");
            // }

            vector<pair<int, float> > similarities;

            similarities = computeSimilarity(user_id);

            // for (int index=0; index < similarities_total[user_id].size(); index++){
            //     similarities.push_back(make_pair(index, similarities_total[user_id][index]));
            // }

            std::sort(similarities.begin(), similarities.end(), sortingfunc);

            int count = 0;
            int topk = 10;
            for (int index = 1; index < similarities.size() && count < topk; index++) {
                int other_user_id = similarities[index].first;
                if (other_user_id == user_id || item_id >= data[other_user_id].size() || data[other_user_id][item_id] == 0.0) {
                    continue; // Skip the target user and unrated items
                }
                predectedRating += data[other_user_id][item_id] * scaledfunc(similarities[index].second, 0, 1, similarities);
                count += 1;
            }
            if (count == 0){
                return predectedRating;
            }
            return (predectedRating / count);
        }
};


int main() {
  CollaborativeFiltering recommender;
  recommender.loadData("train.csv", true);

  cout << recommender.isUB << endl;

  float rating = recommender.predictRating(2199,99);
  std::cout << "Recommend rating " << rating << std::endl;

  std::ifstream test_file("test.csv");
  if (!test_file.is_open()) {
    std::cerr << "Error: Could not open test file" << std::endl;
    return 1;
  }

  // Open the output file
  std::ofstream output_file("predictions.csv");
  if (!output_file.is_open()) {
    std::cerr << "Error: Could not open output file" << std::endl;
    return 1;
  }

  // Read the test data and make predictions
  std::string line;

  std::getline(test_file, line);

  while (std::getline(test_file, line)) {

    int user_id, item_id;
    int id;
    stringstream ss(line);
    getline(ss, line, ',');
    id = stoi(line);
    getline(ss, line, ',');
    user_id = stoi(line);
    getline(ss, line, ',');
    item_id = stoi(line);

    float predicted_rating = 0.0;
    try {
      if (recommender.isUB == true){
        predicted_rating = recommender.predictRating(user_id, item_id);
      }
      else {
        predicted_rating = recommender.predictRating(item_id, user_id);
      }
      output_file << id << "," << predicted_rating << std::endl;
    } catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
  }

  // Close the test and output files
  test_file.close();
  output_file.close();
  return 0;
}