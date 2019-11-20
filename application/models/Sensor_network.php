<?php

defined('BASEPATH') or exit('No direct script access allowed');

class Sensor_network extends CI_Model
{

    public function __construct()
    {
        parent::__construct();
        $this->load->database();
        
    }

    public function get_projects($user_id)
    {
         $query = $this->db->query("Select * from sn_project where user_id = '$user_id'");
         return $query->result();
       
    }

    public function get_nodes($project_id)
    {
         $query = $this->db->query("Select * from sn_nodes where project_id = '$project_id'");
         return $query->result();
       
    }

    public function register($response)
    {
        $this->db->insert('sn_registeration', $response); 

        if ($this->db->affected_rows() > 0)
        {
            echo 'Success';
        } else
        {

        echo 'Failure';
        }
       
    }

    public function checkUsername($username){

        $this->db->select('*');
        $this->db->from('sn_registeration');
        $this->db->where('username', $username);

        $query = $this->db->get();
        $data=$query->result();


        if ($this->db->affected_rows() > 0)
        {
            return 0;
        } else
        {

        return 1;
        }
    }

       public function checkEmail($email){

        $this->db->select('*');
        $this->db->from('sn_registeration');
        $this->db->where('email', $email);

        $query = $this->db->get();
        $data=$query->result();


        if ($this->db->affected_rows() > 0)
        {
            return 0;
        } else
        {

        return 1;
        }
    }

    public function login($response)
    {
        $this->db->select('*');
        $this->db->from('sn_registeration');
        $this->db->where('username', $response['username']);
        $this->db->where('password', $response['password']);

        $query = $this->db->get();
        $data=$query->result();
       $arr=array(array('user_id' => $data[0]->user_id));

        if ($this->db->affected_rows() == 1)
        {
            echo json_encode($arr);
        } else
        {
           echo 'Failure';
        }
       
    }
    
}
