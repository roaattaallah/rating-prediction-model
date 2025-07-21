# Rating Prediction Model

This repository implements a collaborative filtering model in C++ to predict user-item ratings, typically for recommendation systems. The model uses user-based collaborative filtering to estimate ratings for user-item pairs.

## Project Structure

- `my_model_code.cpp` — Main C++ source code implementing collaborative filtering.
- `train.csv` — Training data containing user-item-rating triples.
- `test.csv` — Test data for which predictions are to be made.
- `sample_submission.csv` — Example of the expected output format for predictions.

## Data Format

### `train.csv`
- **Format:** (inferred from code)
  ```
  UserID,ItemID,Rating
  1,10,4.0
  2,15,3.5
  ...
  ```
  - `UserID`: Integer identifier for the user.
  - `ItemID`: Integer identifier for the item.
  - `Rating`: Float value representing the user's rating for the item.

### `test.csv`
- **Format:**
  ```
  ID,UserID,ItemID
  0,4230,896
  1,2199,99
  ...
  ```
  - `ID`: Row identifier.
  - `UserID`: Integer identifier for the user.
  - `ItemID`: Integer identifier for the item.

### `sample_submission.csv`
- **Format:**
  ```
  ID,Predicted
  0,3.5
  1,4.0
  ...
  ```
  - `ID`: Row identifier (matches `test.csv`).
  - `Predicted`: Predicted rating for the user-item pair.

## How It Works

- The program loads the training data and builds a user-item rating matrix.
- For each user-item pair in the test set, it predicts the rating using user-based collaborative filtering.
- The predictions are written to `predictions.csv` in the same format as `sample_submission.csv`.

## Usage

### 1. Compile the Code

```bash
g++ -o rating_predictor my_model_code.cpp
```

### 2. Run the Program

```bash
./rating_predictor
```

- The program expects `train.csv` and `test.csv` to be present in the same directory.
- Output will be written to `predictions.csv`.

## Customization

- The code can be modified to use item-based collaborative filtering by changing the `user_based` parameter in the `loadData` function call in `main()`.
- You can adjust the number of neighbors (`topk`) in the `predictRating` function for different results.

## Notes

- The code prints out some statistics about the most active users and items.
- Error handling is included for missing files.

## License

[Specify your license here] 