import sys
import pandas as pd
import simplekml

# Remove duplicate MAC addresses.
def remove_duplicates(in_df):
    my_df = in_df.drop_duplicates(subset=['MAC'])
    print('Removed duplicate MAC addresses\n')
    return my_df

# Add decimals to the GPS coordinates.
def format_coordinates(in_df):
    in_df.loc[:, 'lat'] = in_df['lat'].astype(float)
    in_df.loc[:, 'lat'] = in_df['lat'].apply(lambda x: x / 10**7)
    in_df.loc[:, 'lon'] = in_df['lon'].astype(float)
    in_df.loc[:, 'lon'] = in_df['lon'].apply(lambda x: x / 10**7)
    print('Formatted the GPS coordinates\n')

# Create a KML file from the pandas dataframe
def create_kml(in_df, kml_file):
    kml = simplekml.Kml()
    for index, row in in_df.iterrows():
        kml.newpoint(name=row['SSID'], coords=[(row['lon'], row['lat'])], description=[(row['MAC'] +
         '   ' + row['AuthMode'] +
         '   ' + str(row['Channel']) +
         '   ' + str(row['RSSI']))])
    kml.save(kml_file)
    print(f'File written: {kml_file}')


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: warscrub.py <input_file.csv> <output_file.kml")
    else:
        input_file = sys.argv[1]
        output_file = sys.argv[2]
        
        df = pd.read_csv(input_file, on_bad_lines='skip')
        clean_df = remove_duplicates(df)
        format_coordinates(clean_df)
        create_kml(clean_df, output_file)

