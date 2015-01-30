<?php
interface Changeset {
    function getBSONChanges();
}
trait foobar {
    protected $_origin;

    protected function _getData() {
        return get_object_vars($this);
    }
    protected function _getOriginState() {
        var_dump($this->_origin, __LINE__);
        return $this->_origin;
    }
    protected function _initState($props) {
        $this->_origin = $props;
        var_dump($this->_origin, __LINE__);
    }

    function _getBSONChangesRecursive($current, $origin) {
        var_dump(__LINE__);
        $changes = array();
        debug_print_backtrace();
        foreach($current as $k => $v) {
            if (is_object($v)) {
                var_dump(__LINE__);
                if ($v instanceof Changeset) {
                    $ret = $v->getBSONChanges();
                    if ($ret) {
                        $changes[$k] = $ret;
                    }
                    continue;
                }
                if ($v !== $origin[$k]) {
                    $changes[$k] = $v;
                }
                continue;
            }
            if (!isset($origin[$k])) {
                var_dump(__LINE__);
                $changes[$k] = $v;
                continue;
            }
            if (is_array($v)) {
                var_dump(__LINE__);
                var_dump(__LINE__);
                var_dump($v, $origin[$k]);
                $ret = $this->_getBSONChangesRecursive($v, $origin[$k]);
                if ($ret) {
                    var_dump(__LINE__);
                    $changes[$k] = $ret;
                }
                var_dump(__LINE__);
                continue;
            }
            if (is_scalar($v)) {
                var_dump(__LINE__);
                if ($v !== $origin[$k]) {
                    $changes[$k] = $v;
                }
                continue;
            }
            var_dump(__LINE__);
        }
        return $changes;
    }
    function getBSONChanges() {
        $origin = $this->_getOriginState();
        var_dump($this->_origin, __LINE__);
        var_dump($origin, __LINE__);exit;
        $current = $this->_getData();
        $changes = $this->_getBSONChangesRecursive($current, $origin);
        return $changes;
    }
}

class Blog implements BSON\Persistable {
    use foobar;

    protected $title;
    protected $entry;
    protected $author;
    protected $comments = array();

    function __construct($title, $entry) {
        $this->title = $title;
        $this->entry = $entry;
    }
    function setAuthor(Person $author) {
        $this->author = $author;
    }
    function addComment(Comment $comment) {
        $this->comments[] = $comment;
    }

    function bsonSerialize() {
        $data = get_object_vars($this);
        return $data;
    }
    function bsonUnserialize(array $data) {
        $this->_initState($data);

        $this->title    = $data["title"];
        $this->entry    = $data["entry"];
        $this->author   = $data["author"];
        $this->comments = (array)$data["comments"];
    }
}

class Person implements BSON\Persistable {
    use foobar;

    protected $_id;
    protected $name;
    protected $email;

    function __construct($name, $email) {
        /* Generate our own ObjectID */
        $this->_id   = new BSON\ObjectID();

        $this->name  = $name;
        $this->email = $email;
    }
    function bsonSerialize() {
        $data = get_object_vars($this);
        return $data;
    }
    function bsonUnserialize(array $data) {
        $this->_initState($data);

        $this->_id   = $data["_id"];
        $this->name  = $data["name"];
        $this->email = $data["email"];
    }
}

class Comment implements BSON\Persistable {
    use foobar;
    protected $person;
    protected $message;

    function __construct(Person $person, $message) {
        $this->person  = $person;
        $this->message = $message;
    }
    function bsonSerialize() {
        $data = get_object_vars($this);
        return $data;
    }
    function bsonUnserialize(array $data) {
        $this->_initState($data);

        $this->person  = $data["person"];
        $this->message = $data["message"];
    }
}



$blog = new Blog("MOS: Mongo Object Storage", "MOS is an amazing object storage that does amazing things");
$hannes = new Person("Hannes Magnusson", "bjori@php.net");
$blog->setAuthor($hannes);

try {
    $wc = new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY);
    $manager = new MongoDB\Driver\Manager("mongodb://localhost");
    /* Remove any previous people */
    $result = $manager->executeDelete("myapp.blogs", array());

    $result = $manager->executeInsert("myapp.blogs", $blog, $wc);
} catch(Exception $e) {
    echo $e->getMessage(), "\n";
    exit;
}

$rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY_PREFERRED);
$query = new MongoDB\Driver\Query(array());
$cursor = $manager->executeQuery("myapp.blogs", $query, $rp);
foreach($cursor as $blog) {
    $commentor = new Person("Anonymous", "anon@example.com");
    $comment = new Comment($commentor, "This is amazing blog entry!");
    $blog->addComment($comment);
    //var_dump($blog);
    var_dump($blog->getBSONChanges());
}





