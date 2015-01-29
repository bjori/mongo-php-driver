<?php
trait ObjectDiff {
    protected $_origin;

    protected function _getData() {
        return get_object_vars($this);
    }
    protected function _getOriginState() {
        return $this->_origin;
    }
    protected function _initState($props) {
        $this->_origin = $props;
    }

    function getBSONChanges() {
        $origin = $this->_getOriginState();
    }
}

class Person implements BSON\Persistable {
    use ObjectDiff;

    protected $_id;
    protected $name;
    protected $age;
    protected $address = array();
    protected $friends = array();
    protected $secret = "none";

    function __construct($name, $age) {
        $this->name    = $name;
        $this->age     = $age;
        $this->address = array();
        $this->secret  = "$name confidential info";
        /* Pregenerate our ObjectID */
        $this->_id     = new BSON\ObjectID();
    }
    function addAddress(Address $address) {
        $this->address[] = $address;
    }
    function addFriend(Person $friend) {
        $this->friends[] = $friend;
    }
    function bsonSerialize() {
        return array(
            "_id"     => $this->_id,
            "name"    => $this->name,
            "age"     => $this->age,
            "address" => $this->address,
            "friends" => $this->friends,
        );
    }
    function bsonUnserialize(array $data) {
        $this->_initState($data);

        $this->_id     = $data["_id"];
        $this->name    = $data["name"];
        $this->age     = $data["age"];
        $this->address = (array)$data["address"];
        $this->friends = (array)$data["friends"];
    }
}

class Address implements BSON\Persistable {
    use ObjectDiff;
    protected $zip;
    protected $country;

    function __construct($zip, $country) {
        $this->zip = $zip;
        $this->country = $country;
    }
    function bsonSerialize() {
        return array(
            "zip"     => $this->zip,
            "country" => $this->country,
        );
    }
    function bsonUnserialize(array $data) {
        $this->_initState($data);

        $this->zip = $data["zip"];
        $this->country = $data["country"];
    }
}



$hannes = new Person("Hannes", 31);
$sunnyvale = new Address(94086, "USA");
$kopavogur = new Address(200, "Iceland");
$hannes->addAddress($sunnyvale);
$hannes->addAddress($kopavogur);

$mikola = new Person("Jeremy", 21);
$michigan = new Address(48169, "USA");
$mikola->addAddress($michigan);

$hannes->addFriend($mikola);
#$mikola->addFriend($hannes);

$didier = new Person("Drogba", 36);


try {
    $wc = new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY);
    $manager = new MongoDB\Driver\Manager("mongodb://192.168.112.10:2000");
    /* Remove any previous people */
    $result = $manager->executeDelete("congress.people", array());

    $result = $manager->executeInsert("congress.people", $hannes, $wc);
    $result = $manager->executeInsert("congress.people", $mikola, $wc);
    echo "Hannes & Jeremy have been inserted\n";
} catch(Exception $e) {
    echo $e->getMessage(), "\n";
    exit;
}

$rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY_PREFERRED);
$query = new MongoDB\Driver\Query(array());
$cursor = $manager->executeQuery("congress.people", $query, $rp);
foreach($cursor as $person) {
    $person->addFriend($didier);
    var_dump($person->getChanges());
}





