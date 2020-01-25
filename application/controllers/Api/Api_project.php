<?php 
defined('BASEPATH') OR exit('No direct script access allowed');
class Api_project extends CI_Controller {
	public function __construct()
    {
        parent::__construct();
        $this->load->model('Sensor_network');
        
    }
	/**
	 * Index Page for this controller.
	 *
	 * Maps to the following URL
	 * 		http://example.com/index.php/welcome
	 *	- or -
	 * 		http://example.com/index.php/welcome/index
	 *	- or -
	 * Since this controller is set as the default controller in
	 * config/routes.php, it's displayed at http://example.com/
	 *
	 * So any other public methods not prefixed with an underscore will
	 * map to /index.php/welcome/<method_name>
	 * @see https://codeigniter.com/user_guide/general/urls.html
	 */
	public function index()
	{
		
		$this->get_project();
	}
	public function get_project(){
		$user_id = $this->uri->segment(4);
		$data = array();
		$data['result'] = $this->Sensor_network->get_projects($user_id);
		$data = json_encode($data['result'],JSON_UNESCAPED_UNICODE);
		echo $data;
	}
	public function get_node(){
		$project_id = $this->uri->segment(4);
		$data = array();
		$data['result'] = $this->Sensor_network->get_nodes($project_id);
		$data = json_encode($data['result'],JSON_UNESCAPED_UNICODE);
		echo $data;
	}
	public function register(){
		$data = array();
		$data['name']=$this->input->post('name');
		$data['username']=$this->input->post('username');
		$data['email']=$this->input->post('email');
		$data['password']=$this->input->post('password');
		$userCheck = $this->Sensor_network->checkUsername($data['username']);
		if($userCheck==0){
			echo 'username already exist';
		}
		else if($userCheck==1){
			$emailCheck = $this->Sensor_network->checkEmail($data['email']);
			
			if($emailCheck==0){
				echo 'email already exist';
			}
			else{
				$dataResponse = $this->Sensor_network->register($data);
			}
		}
	}
	public function login(){
		$data = array();
		$data['username']=$this->input->post('username');
		$data['password']=$this->input->post('password');
		$dataResponse = $this->Sensor_network->login($data);
	
	}

	public function insertData(){
		 $data = array();
		 $arduinoData=$this->input->post('data');
		 //print_r($arduinoData);die();
		 $str_arr = explode (",", $arduinoData); 
		
		 $nodeID = explode ("=", $str_arr[0]);
		 $project_Id = explode ("=", $str_arr[1]);
		 $longTemp = explode ("-", $str_arr[2]);
		 $latTemp = explode ("=", $longTemp[0]);

		 $str=array();
		 // $str['lat']= $latTemp[1];
		 // $str['long']= $longTemp[1];
		 $latitude="31.447";
		 $longitude="74.2678";

		 $digits = 5;
		 $rndNum= rand(pow(10, $digits-1), pow(10, $digits)-1);

		 if($nodeID[1] == 2){
		 	 $str['lat']= "31.447186";
		 	 $str['long']= "74.267291";
		 }
		 else{
		 	//$str['lat']= $latTemp[1].$rndNum;
		 $str['lat']= $latitude.$rndNum;

		 $rndNum= rand(pow(10, $digits-1), pow(10, $digits)-1);
		 //$str['long']= $longTemp[1].$rndNum;
		 $str['long']= $longitude.$rndNum;

		 }

		 

		 $str1 = implode(",",$str);
	
		 $sensor_1 = explode ("=", $str_arr[3]);
		 $sensor_2 = explode ("=", $str_arr[4]);

		 $turb = explode ("-", $str_arr[4]);
		 $ph = explode ("=", $turb[0]);

		 $str3=array();
		 $str3['ph']= $ph[1];
		 $str3['turb']= $turb[1];

		 print_r($sensor_2);
		 //die();
		 $sensor_3 = explode ("=", $str_arr[5]);
		 $sensor_4 = explode ("=", $str_arr[6]);
		 
		 $data['node_id']=$nodeID[1];
		 $data['project_id']=$project_Id[1];
		 $data['latlng']=$str1;
		 $data['sensor_1']=$sensor_1[1];
		 $data['sensor_2']=$turb[1];
		 $data['sensor_3']=$ph[1];
		 $data['sensor_4']=$sensor_4[1];

		if(!empty($data)){
			$dataResponse = $this->Sensor_network->insertData($data);
		}
		else{
			echo 'error sending data';
		}
	}
}