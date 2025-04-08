program TypesAndRecordsExample;

type
  TPoint = record
    x: Integer;
    y: Integer;
  end;

  // Defining a type alias for a string that could be used for a person's name
  TName = string;

  // Defining a record type for a person
  TPerson = record
    name: TName;
    age: Integer;
    locationx: Integer;
    locationy: Integer;
  end;

var
  person1: TPerson;
  person2: TPerson;

// Function to calculate the distance between two points
function CalculateDistance(p1, p2: Integer): Real;
begin
  CalculateDistance := p1 - p2;
end;

// Procedure to print details about a person
procedure PrintPersonDetails(person: TPerson);
begin
  WriteLn('Name: ', person.name);
  WriteLn('Age: ', person.age);
  WriteLn('Location: (', person.locationx, ', ', person.locationy, ')');
end;

procedure UpdateAge(person: TPerson; newAge: Integer);
begin
  person.age := newAge;
end;

// Procedure to move a person to a new location
procedure MovePerson(person: TPerson; newX, newY: Integer);
begin
  person.locationx := newX;
  person.locationy := newY;
end;

begin
  // Assigning values to the first person record
  person1.name := "Alice";
  person1.age := 25;
  person1.locationx := 10;
  person1.locationy := 20;

  // Assigning values to the second person record
  person2.name := "Bob";
  person2.age := 30;
  person2.locationx := 15;
  person2.locationy := 25;

  // Displaying the values of the records
  WriteLn('Person 1 Details:');
  PrintPersonDetails(person1);
  
  WriteLn('Person 2 Details:');
  PrintPersonDetails(person2);

  // Calculate and display the distance between person1 and person2
  distance := CalculateDistance(person1.locationx, person2.locationx);
  WriteLn('Distance between Person 1 and Person 2: ', distance);

  // Update person1's age and move person2 to a new location
  UpdateAge(person1, 26);
  MovePerson(person2, 20, 30);

  WriteLn;
  WriteLn('Updated Details:');

  // Display updated details after changes
  WriteLn('Person 1 Details (Updated):');
  PrintPersonDetails(person1);
  
  WriteLn('Person 2 Details (Updated):');
  PrintPersonDetails(person2);
end.
