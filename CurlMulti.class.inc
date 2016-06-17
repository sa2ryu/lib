<?php
/*
   curlmultiクラス
   $url = array(
   'name' => 'http://aaa',
   );
   $opt = array(
   CURL_OPT => true,
   );
   $opt_list = array(
   'name' => array(
   CURL_OPT => true,
   )
   );
   $result = array(
   'name' => 'aaaa'
   );
 */

class CurlMulti{
	// url list
	private $url = array();
	// curl default
	private $opt = array();
	// curl opt list
	private $opt_list = array();
	// result list
	private $res = array();
	// info list
	private $info = array();
	// err list
	private $err = array();

	// {{{ public function exec()
	public function exec($timeout = 3){
		$mh = curl_multi_init();
		$ch = array();
		// urlの設定
		foreach($this->url as $k => $v){
			$ch[$k] = curl_init($v);
			curl_setopt($ch[$k],CURLOPT_TIMEOUT,$timeout);
			curl_setopt($ch[$k],CURLOPT_RETURNTRANSFER,true);
			if(count($this->opt)){
				curl_setopt_array($ch[$k],$this->opt);
			}
			if(isset($this->opt_list[$k]) && count($this->opt_list[$k])){
				curl_setopt_array($ch[$k],$this->opt_list[$k]);
			}
			curl_multi_add_handle($mh, $ch[$k]);
		}
		// 実行
		$active = null;
		do{
			$mrc = curl_multi_exec($mh, $active);
		}while($mrc == CURLM_CALL_MULTI_PERFORM);

		while ($active && $mrc == CURLM_OK) {
			if (curl_multi_select($mh) != -1) {
				do {
					$mrc = curl_multi_exec($mh, $active);
				}while($mrc == CURLM_CALL_MULTI_PERFORM);
			}
		}
		// 取得
		foreach($this->url as $k => $v){
			if ($err = curl_error($ch[$k])) {
				$this->err[$k] = $err;
			}
			$this->info[$k] = curl_getinfo($ch[$k]);
			$this->res[$k] = curl_multi_getcontent($ch[$k]);
			curl_multi_remove_handle($mh,$ch[$k]);
			curl_close($ch[$k]);
		}
		curl_multi_close($mh);
	}
	// }}}

	// {{{ public function clear()
	public function clear(){
		$this->url = array();
		$this->opt = array();
		$this->opt_list = array();
		$this->res = array();
	}
	// }}}

	// {{{ public function setUrl($array)
	public function setUrl($array){
		$this->url = $array;
	}
	// }}}

	// {{{ public function setOpt($array)
	public function setOpt($array){
		$this->opt = $array;
	}
	// }}}

	// {{{ public function setOpt($array)
	public function setOptList($array){
		$this->opt_list = array_merge($this->opt_list,$array);
	}
	// }}}

	// {{{ public function getRes()
	public function getRes(){
		return $this->res;
	}
	// }}}

	// {{{ public function getInfo()
	public function getInfo(){
		return $this->info;
	}
	// }}}

	// {{{ public function getErr()
	public function getErr(){
		return $this->err;
	}
	// }}}

}
/* vim:set foldmethod=marker: */
?>
