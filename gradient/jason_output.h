#include <iostream>
void SINGLE_POINT(vect<2> x)
{
	cout << "var geojsonFeature = {" << endl;
    cout << "\"type\": \"Feature\" ," << endl;
    cout << "\"properties\": {" << endl;
    cout << "\"name\": \"Coors Field\"," << endl;
    cout << "\"amenity\": \"Baseball Stadium\"," << endl;
    cout << "\"popupContent\": \"This is where the Rockies play!\"" << endl;
    cout << "}," << endl;
    cout << "\"geometry\": {" << endl;
	cout << "    \"type\": \"Point\", \n";
	cout << "    \"coordinates\": [" << x.v[0] << "," << x.v[1] << "]\n";
	cout << "}\n";
}

void NEW_GROUP()
{
	cout << "\"geometry\": {" << endl;
	cout << "    \"type\": \"MultiLineString\", \n";
	cout << "    \"coordinates\": [\n";
}

void END_GROUP()
{
	cout << "    ]\n}\n";
}

void NEW_LINE()
{
	cout << "        [";
}
void END_LINE()
{
	cout << "],\n";
}
void END_LAST_LINE()
{
	cout << "]\n";;
}

void NEW_POINT(vect<2> x)
{
	cout << "[" << x.v[0] << "," << x.v[1] << "], ";
//	dots++;
}

void LAST_POINT(vect<2> x)
{
	cout << "[" << x.v[0] << "," << x.v[1] << "]";
//	dots++;
}